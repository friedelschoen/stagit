#include "common.h"

#include "compat.h"

#include <ctype.h>
#include <err.h>
#include <errno.h>
#include <limits.h>
#include <sys/stat.h>


/* Handle read or write errors for a FILE * stream */
void checkfileerror(FILE* fp, const char* name, int mode) {
	if (mode == 'r' && ferror(fp))
		errx(1, "read error: %s", name);
	else if (mode == 'w' && (fflush(fp) || ferror(fp)))
		errx(1, "write error: %s", name);
}


int mkdirp(const char* path) {
	char tmp[PATH_MAX], *p;

	if (strlcpy(tmp, path, sizeof(tmp)) >= sizeof(tmp))
		errx(1, "path truncated: '%s'", path);
	for (p = tmp + (tmp[0] == '/'); *p; p++) {
		if (*p != '/')
			continue;
		*p = '\0';
		if (mkdir(tmp, S_IRWXU | S_IRWXG | S_IRWXO) < 0 && errno != EEXIST)
			return -1;
		*p = '/';
	}
	if (mkdir(tmp, S_IRWXU | S_IRWXG | S_IRWXO) < 0 && errno != EEXIST)
		return -1;
	return 0;
}

/* Escape characters below as HTML 2.0 / XML 1.0, ignore printing '\r', '\n' */
void xmlencodeline(FILE* fp, const char* s, size_t len) {
	size_t        i = 0;
	unsigned char c;
	uint32_t      codepoint;

	while (i < len && *s) {
		c = (unsigned char) *s;

		// Handle ASCII characters
		if (c < 0x80) {
			switch (c) {
				case '<':
					fputs("&lt;", fp);
					break;
				case '>':
					fputs("&gt;", fp);
					break;
				case '\'':
					fputs("&#39;", fp);
					break;
				case '&':
					fputs("&amp;", fp);
					break;
				case '"':
					fputs("&quot;", fp);
					break;
				case 0x0B:
				case 0x0C:
					break;
				case '\r':
				case '\n':
					break; /* ignore LF */
				default:
					if (isprint(c) || isblank(c))
						putc(c, fp);
					else
						fprintf(fp, "&#%d;", c);
			}
			s++;
			i++;
		}
		// Handle multi-byte UTF-8 sequences
		else if (c < 0xC0) {
			// Invalid continuation byte at start, print as is
			fprintf(fp, "&#%d;", c);
			s++;
			i++;
		} else {
			// Decode UTF-8 sequence
			const unsigned char* start     = (unsigned char*) s;
			int                  remaining = 0;

			if (c < 0xE0) {
				// 2-byte sequence
				remaining = 1;
				codepoint = c & 0x1F;
			} else if (c < 0xF0) {
				// 3-byte sequence
				remaining = 2;
				codepoint = c & 0x0F;
			} else if (c < 0xF8) {
				// 4-byte sequence
				remaining = 3;
				codepoint = c & 0x07;
			} else {
				// Invalid start byte, print as is
				fprintf(fp, "&#%d;", c);
				s++;
				i++;
				continue;
			}

			// Process continuation bytes
			while (remaining-- && *(++s)) {
				c = (unsigned char) *s;
				if ((c & 0xC0) != 0x80) {
					// Invalid continuation byte, print original bytes as is
					while (start <= (unsigned char*) s) {
						fprintf(fp, "&#%d;", *start++);
					}
					s++;
					i++;
					break;
				}
				codepoint = (codepoint << 6) | (c & 0x3F);
			}

			if (remaining < 0) {
				// Successfully decoded UTF-8 character, output as numeric reference
				fprintf(fp, "&#%u;", codepoint);
				s++;
				i++;
			}
		}
	}
}

void normalize_path(char* path) {
	char* src = path;
	char* dst = path;

	while (*src) {
		// Handle "//" by skipping over it
		if (src[0] == '/' && src[1] == '/') {
			src++;
			continue;
		}
		// Handle "/./" by skipping over it
		if (src[0] == '/' && src[1] == '.' && src[2] == '/') {
			src += 2;
			continue;
		}
		// Handle "/../" by removing the last component
		if (src[0] == '/' && src[1] == '.' && src[2] == '.' && src[3] == '/') {
			// Backtrack to the previous directory in dst
			if (dst != path) {
				dst--;
				while (dst > path && *dst != '/') {
					dst--;
				}
			}
			src += 3;
			continue;
		}
		// Copy the current character to the destination
		*dst++ = *src++;
	}
	// Null-terminate the result
	*dst = '\0';
}

void unhide_path(char* path) {
	for (char* chr = path; *chr; chr++) {
		if (*chr == '.' && (chr == path || chr[-1] == '/') && chr[1] != '/')
			*chr = '-';
	}
}
