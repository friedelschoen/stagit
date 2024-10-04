#include "config.h"

/* Configuration keys for the configuration parser. */
struct config config_keys[] = {
	/* Site information settings */
	{ "name", ConfigString, &sitename },
	{ "description", ConfigString, &sitedescription },
	{ "footer", ConfigString, &footertext },

	/* File and path settings */
	{ "favicon", ConfigString, &favicon },
	{ "favicontype", ConfigString, &favicontype },
	{ "logoicon", ConfigString, &logoicon },
	{ "stylesheet", ConfigString, &stylesheet },
	{ "highlightcmd", ConfigString, &highlightcmd },
	{ "colorscheme", ConfigString, &colorscheme },
	{ "pinfiles", ConfigString, &extrapinfiles },

	/* Limits for commits and file sizes */
	{ "limit/commits", ConfigInteger, &maxcommits },
	{ "limit/filesize", ConfigInteger, &maxfilesize },

	/* Files and output configurations */
	{ "files/index", ConfigString, &indexfile },
	{ "files/log", ConfigString, &logfile },
	{ "files/files", ConfigString, &treefile },
	{ "files/json", ConfigString, &jsonfile },
	{ "files/commit-atom", ConfigString, &commitatomfile },
	{ "files/tag-atom", ConfigString, &tagatomfile },

	/* Copy settings */
	{ "copy/stylesheet", ConfigString, &copystylesheet },
	{ "copy/favicon", ConfigString, &copyfavicon },
	{ "copy/logoicon", ConfigString, &copylogoicon },

	{ 0 }, /* Terminating the list */
};

/* --- Site information settings --- */

/* The title of the site, which will be displayed in the HTML header or as the page title. */
const char* sitename = "My Repositories";

/* A description of the site, typically shown in metadata or as a subheading. */
const char* sitedescription = "";

/* Footer text that will appear at the bottom of every generated HTML page. */
const char* footertext = "Generated by <i><code>gitin</code></i>!";

/* --- File and path settings --- */

/* Path to the config files */
const char* configfile = "gitin.conf";

/* Path to the favicon file, which will be used in the HTML output. */
const char* favicon = "favicon.svg";

/* The MIME type of the favicon file, indicating the type of image (SVG in this case). */
const char* favicontype = "image/svg+xml";

/* Path to the logo icon file, which will be displayed in the header of the HTML page. */
const char* logoicon = "logo.svg";

/* Path to the stylesheet (CSS file) used for styling the generated HTML. */
const char* stylesheet = "style.css";

/* Command to execute for highlighting syntax in files within the HTML output. The colorscheme will
 * be substituted into this command. */
const char* highlightcmd =
    "chroma --html --html-only --html-lines --html-inline-styles --style=$scheme --lexer=$type";

/* Color scheme to use for syntax highlighting in the HTML output. */
const char* colorscheme = "pastie";

int filesperdirectory = 1;

const char* pinfiles[] = {
	"README",          "README.md",       "CONTRIBUTING",
	"CONTRIBUTING.md", "CHANGELOG",       "CHANGELOG.md",
	"LICENSE",         "LICENSE.md",      "COPYING",
	"COPYING.md",      "CODE_OF_CONDUCT", "CODE_OF_CONDUCT.md",
	"SECURITY",        "SECURITY.md",     NULL,
};

const char* extrapinfiles = NULL;

/* --- Commit and log settings --- */

/* Maximum number of commits to display in the log view. A value of -1 means no limit. */
long long maxcommits = -1;

/* --- File size settings --- */

/* Maximum file size (in bytes) that will be processed for syntax highlighting. Files larger than
 * this size (1MB) will be skipped. */
long long maxfilesize = 1e+6;    // 1MB

/* --- Output file names --- */

/* Name of the root index file, typically the home page of the generated site. */
const char* indexfile = "index.html";

/* Name of the file containing the commit and branch/tag log. */
const char* logfile = "log.html";

/* Name of the file that will display the file tree. */
const char* treefile = "tree.html";

/* Name of the JSON file containing commit data. */
const char* jsonfile = "commits.json";

/* Name of the file used for Atom feeds of commits. */
const char* commitatomfile = "atom.xml";

/* Name of the file used for Atom feeds of tags or branches. */
const char* tagatomfile = "tags.xml";

/* --- Copy settings --- */

/* Optional path to copy the stylesheet (CSS file) used for the site. */
const char* copystylesheet = NULL;

/* Optional path to copy the favicon. */
const char* copyfavicon = NULL;

/* Optional path to copy the logo icon. */
const char* copylogoicon = NULL;
