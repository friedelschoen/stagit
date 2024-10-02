#include "config.h"

#include "writer.h"


struct config config_keys[] = {
	{ "name", ConfigString, &sitename },
	{ "description", ConfigString, &sitedescription },
	{ "footer", ConfigString, &footertext },
	{ "favicon", ConfigString, &favicon },
	{ "favicontype", ConfigString, &favicontype },
	{ "logoicon", ConfigString, &logoicon },
	{ "stylesheet", ConfigString, &stylesheet },
	{ "highlightcmd", ConfigString, &highlightcmd },
	{ "colorscheme", ConfigString, &colorscheme },
	{ "pinfiles", ConfigString, &pinfilesstr },
	{ "limit/commits", ConfigInteger, &maxcommits },
	{ "limit/filesize", ConfigInteger, &maxfilesize },
	{ "files/index", ConfigString, &indexfile },
	{ "files/log", ConfigString, &logfile },
	{ "files/files", ConfigString, &treefile },
	{ "files/json", ConfigString, &jsonfile },
	{ "files/commit-atom", ConfigString, &commitatomfile },
	{ "files/tag-atom", ConfigString, &tagatomfile },
	{ "copy/stylesheet", ConfigString, &copystylesheet },
	{ "copy/favicon", ConfigString, &copyfavicon },
	{ "copy/logoicon", ConfigString, &copylogoicon },
	{ 0 },
};

/* Site information */

/* The title of the site, which will be displayed in the HTML header or as the page title. */
const char* sitename = "My Repositories";

const char* sitedescription = "";

/* Footer text that will appear at the bottom of every generated HTML page. */
const char* footertext = "Generated by <i><code>gitin</code></i>!";

/* File and path configurations */

/* Path to the favicon file, which will be used in the HTML output. */
const char* favicon = "favicon.svg";

/* The MIME type of the favicon file, indicating that it's an SVG image. */
const char* favicontype = "image/svg+xml";

/* Path to the logo icon file, which will be used in the header of the HTML page. */
const char* logoicon = "logo.svg";

/* Path to the stylesheet (CSS file) used for styling the generated HTML. */
const char* stylesheet = "style.css";

/* Command to execute for highlighting a file's syntax in the HTML output. The colorscheme will be substituted into the
 * command. */
const char* highlightcmd = "chroma --html --html-only --html-lines --html-inline-styles --style=$scheme --lexer=$type";

/* Color scheme to use for syntax highlighting in the HTML output. */
const char* colorscheme = "pastie";

/* Pin file settings */

/* List of files (by name) that will be pinned in the header of the HTML page, typically important files like licenses
 * or README files. */
const char* pinfilesstr = "LICENSE LICENSE.md COPYING README README.md";

const char* pinfiles[MAXPINS];
int         npinfiles;

/* Commit and log settings */

/* Maximum number of commits to display in the log view. A value of -1 indicates that no limit is set. */
long long maxcommits = -1;

/* File size settings */

/* Maximum file size (in bytes) that will be highlighted or processed. Files larger than this size (1MB) will be
 * skipped. */
long long maxfilesize = 1e+6;    // 1MB

/* File names */

/* Name of configuration file */
const char* configfile = "gitin.conf";

/* Name of root index file */
const char* indexfile = "index.html";

/* Name of commit and branch/tag-log */
const char* logfile = "log.html";

/* Name of filetree */
const char* treefile = "tree.html";

/* Name of JSON-file for commits */
const char* jsonfile = "commits.json";

/* Name of commit Atom */
const char* commitatomfile = "atom.xml";

/* Name of tag/branch Atom */
const char* tagatomfile = "tags.xml";

const char* copystylesheet = NULL;
const char* copyfavicon    = NULL;
const char* copylogoicon   = NULL;
