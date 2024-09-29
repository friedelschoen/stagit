#include "common.h"
#include "config.h"
#include "hprintf.h"
#include "parseconfig.h"
#include "writer.h"

#include <limits.h>
#include <string.h>


void freeheadfiles(struct repoinfo* info) {
	if (info->headfiles != NULL) {
		// Free each string in the list
		for (int i = 0; i < info->headfileslen; i++) {
			free(info->headfiles[i]);
		}
		// Free the list itself
		free(info->headfiles);
		info->headfiles = NULL;
	}

	info->headfileslen   = 0;
	info->headfilesalloc = 0;
}

void writerepo(FILE* index, const char* repodir, const char* destination) {
	struct repoinfo info;
	git_object*     obj = NULL;
	FILE*           fp;
	char            path[PATH_MAX];
	int             i;
	const char*     start;

	memset(&info, 0, sizeof(info));
	info.repodir = repodir;

	info.relpath = 1;
	for (const char* p = repodir + 1; p[1]; p++)
		if (*p == '/')
			info.relpath++;

	start = repodir;
	if (start[0] == '/')
		start++;
	/* use directory name as name */
	strlcpy(info.name, start, sizeof(info.name));
	if (info.name[strlen(info.name) - 1] == '/')
		info.name[strlen(info.name) - 1] = '\0';


	snprintf(info.destdir, sizeof(info.destdir), "%s/%s", destination, info.repodir);
	normalize_path(info.destdir);

	printf("updating '%s' -> %s\n", info.name, info.destdir);

	if (mkdirp(info.destdir, 0777) == -1) {
		hprintf(stderr, "error: unable to create destination directory: %w\n", info.destdir);
		exit(100);
	}
	snprintf(path, sizeof(path), "%s/.gitin/files", info.destdir);
	if (mkdirp(path, 0777) == -1) {
		hprintf(stderr, "error: unable to create .gitin/files directory: %w\n", path);
		exit(100);
	}
	snprintf(path, sizeof(path), "%s/.gitin/archive", info.destdir);
	if (mkdirp(path, 0777) == -1) {
		hprintf(stderr, "error: unable to create .gitin/archive directory: %w\n", path);
		exit(100);
	}
	snprintf(path, sizeof(path), "%s/commit", info.destdir);
	if (mkdirp(path, 0777) == -1) {
		hprintf(stderr, "error: unable to create commit directory: %w\n", path);
		exit(100);
	}

	if (git_repository_open_ext(&info.repo, info.repodir, GIT_REPOSITORY_OPEN_NO_SEARCH, NULL) < 0) {
		hprintf(stderr, "error: unable to open git repository '%s': %gw\n", info.repodir);
		exit(100);
	}

	/* find HEAD */
	if (!git_revparse_single(&obj, info.repo, "HEAD")) {
		info.head = git_object_id(obj);
	}
	git_object_free(obj);

	struct configstate state;
	memset(&state, 0, sizeof(state));
	snprintf(path, sizeof(path), "%s/%s", repodir, configfile);
	normalize_path(path);

	if ((fp = fopen(path, "r"))) {
		while (!parseconfig(&state, fp)) {
			if (!strcmp(state.key, "description"))
				strlcpy(info.description, state.value, sizeof(info.description));
			else if (!strcmp(state.key, "url") || !strcmp(state.key, "cloneurl"))
				strlcpy(info.cloneurl, state.value, sizeof(info.cloneurl));
			else
				hprintf(stderr, "warn: ignoring unknown config-key '%s'\n", state.key);
		}
		fclose(fp);
	}

	/* check pinfiles */
	for (i = 0; i < npinfiles && info.pinfileslen < MAXPINS; i++) {
		snprintf(path, sizeof(path), "HEAD:%s", pinfiles[i]);
		if (!git_revparse_single(&obj, info.repo, path) && git_object_type(obj) == GIT_OBJ_BLOB)
			info.pinfiles[info.pinfileslen++] = pinfiles[i];
		git_object_free(obj);
	}

	if (!git_revparse_single(&obj, info.repo, "HEAD:.gitmodules") && git_object_type(obj) == GIT_OBJ_BLOB)
		info.submodules = ".gitmodules";
	git_object_free(obj);

	writefiles(&info);

	/* log for HEAD */
	snprintf(path, sizeof(path), "%s/index.html", info.destdir);
	if (!(fp = fopen(path, "w"))) {
		hprintf(stderr, "error: unable to open file: %s: %w\n", path);
		exit(100);
	}
	if (verbose)
		fprintf(stderr, "%s\n", path);
	writeheader(fp, &info, 0, info.name, "%y", info.description);
	writerefs(fp, &info);
	fputs("<h2>Commits</h2>\n<table id=\"log\"><thead>\n<tr><td><b>Date</b></td>"
	      "<td class=\"expand\"><b>Commit message</b></td>"
	      "<td><b>Author</b></td><td class=\"num\" align=\"right\"><b>Files</b></td>"
	      "<td class=\"num\" align=\"right\"><b>+</b></td>"
	      "<td class=\"num\" align=\"right\"><b>-</b></td></tr>\n</thead><tbody>\n",
	      fp);

	if (info.head)
		writelog(fp, &info);

	fputs("</tbody></table>", fp);
	writefooter(fp);
	fclose(fp);

	if (index) {
		writeindexline(index, &info);
	}

	/* cleanup */
	git_repository_free(info.repo);
	freeheadfiles(&info);
}
