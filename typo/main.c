#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cmark.h>

#define TAG_TITLE	"{{ title }}"
#define TAG_CONTENT	"{{ content }}"

/* render: render content using the given template.

   this function reads the two files, converts the Markdown in content to HTML,
   and injects it into the placeholder in the template, returning the result. 
   
   template should be a HTML template with a single TAG_CONTENT.

   content should be a Markdown file. 

   returns 0 on success, -1 on failure. caller takes ownership of *result on
   success. */
static int
render(char *template, char *content, char **result);

int
main(int argc, char *argv[])
{
	char *page;

	if (argc != 3) {
		fprintf(stderr, "usage: %s TEMPLATE CONTENT\n", argv[0]);
		return EXIT_FAILURE;
	}

	if (render(argv[1], argv[2], &page) != 0)
		return EXIT_FAILURE;

	printf("%s", page);
	free(page);

	return EXIT_SUCCESS;
}

static int
read(char *path, char **result);

/* md2html: takes ownership of content, even on failure.
   returns 0 with allocated *result, or -1. */
static int
md2html(char *content, char **result);

/* replace: requires exactly one occurrence of a nonempty needle.
   returns 0 with an allocated *result, or -1 leaving on error.
   inputs are not modified; caller must free *result on success. */
static int
replace(char *text, char *needle, char *replacement, char **result);

static int
render(char *template, char *content, char **result)
{
	char *md;

	char *tmpl = NULL,
	     *html = NULL;

	int ok = read(template, &tmpl) == 0
		&& read(content, &md) == 0
		&& md2html(md, &html) == 0
		&& replace(tmpl, TAG_CONTENT, html, result) == 0;

	free(tmpl);
	free(html);

	return ok ? 0 : -1;
}

static int
md2html(char *md, char **result)
{
	char *s;

	s = cmark_markdown_to_html(md, strlen(md), CMARK_OPT_UNSAFE);
	free(md);
	if (s == NULL) {
		fprintf(stderr, "Markdown to HTML conversion failed\n");
		return -1;
	}
	*result = s;
	return 0;
}

static int
replace(char *text, char *needle, char *repl, char **result)
{
	char *loc, *buf;
	size_t prefix, len, repl_len;

	assert(needle[0] != '\0');

	if ((loc = strstr(text, needle)) == NULL) {
		fprintf(stderr, "missing ‘%s’\n", needle);
		return -1;
	}
	if (strstr(loc + 1, needle) != NULL) {
		fprintf(stderr, "repeated ‘%s’\n", needle);
		return -1;
	}

	/* TODO: needs simplification */

	prefix = loc - text;
	len = strlen(text) - strlen(needle);

	repl_len = strlen(repl);

	buf = malloc(len + repl_len + 1);
	assert(buf);

	memcpy(buf, text, prefix);
	memcpy(buf + prefix, repl, repl_len);
	strcpy(buf + prefix + repl_len, loc + strlen(needle));

	*result = buf;
	return 0;
}

/* read: returns 0 with allocated contents in *result, or -1. caller must free
   the contents. see https://stackoverflow.com/a/14002993. */
static int
read(char *path, char **result)
{
	FILE *f;
	long fsize;
	char *buf = NULL;
	size_t count;

	if ((f = fopen(path, "rb")) == NULL) {
		perror(path);
		return -1;
	}

	if (fseek(f, 0, SEEK_END) != 0)
		goto io_error;

	if ((fsize = ftell(f)) < 0)
		goto io_error;
	if ((unsigned long) fsize >= (size_t) -1) {
		fprintf(stderr, "%s: file too large\n", path);
		goto fail;
	}

	if (fseek(f, 0, SEEK_SET) != 0)
		goto io_error;

	buf = malloc((size_t) fsize + 1);
	assert(buf);

	count = fread(buf, 1, (size_t) fsize, f);
	if (ferror(f))
		goto io_error;
	if (count != (size_t) fsize) {
		fprintf(stderr, "%s: unexpected end of file\n", path);
		goto fail;
	}

	if (fclose(f) == EOF) {
		perror(path);
		free(buf);
		return -1;
	}

	buf[fsize] = '\0';
	*result = buf;
	return 0;

io_error:
	perror(path);
fail:
	free(buf);
	fclose(f);
	return -1;
}
