# typo progress

typo is a small C static-site generator for this blog, using libcmark.
We are building it one step at a time and reviewing `main.c` before deciding
whether to split it into multiple files.

## Agreed conventions

- Generator source lives in `typo/` (renamed from `ssg/`).
- `make -C typo` builds `typo/bin/typo`; `/typo/bin/` is ignored by Git.
- The build uses `pkg-config` to find libcmark.
- Use only C89 features, with K&R formatting and function return types on
  separate lines, including forward declarations.
- Define functions after their callers, with forward declarations in call order.
- Keep the file reader (`read`, originally `read_file`) static and use the
  supplied seek/size/read approach.
- `read`, `md2html`, `replace`, and `render` return 0 on success and -1 on failure.
  Each takes a `char **result` output parameter, set to an allocated string on
  success and left unchanged on failure. The function detecting an error prints
  it to stderr; callers propagate failure and clean up. The previously added
  shared reporting helper is absent from the current source.
  `md2html` consumes its input even on failure. `main` returns `EXIT_FAILURE`
  when rendering fails. Failed allocation is an assertion failure; `read` and
  `replace` assert successful allocation.
- Generated site files will live in repository-root `public/`, already ignored
  by Git. Authored content remains in `content/`; the template is
  `web/index.html.tmpl`.

## Completed

1. Read one Markdown file supplied as a command-line argument.
   `read` seeks to determine the size, allocates a terminated string,
   reads the bytes, reports file errors, and asserts successful allocation.
2. Render Markdown with libcmark and print HTML to stdout.
   The current `md2html` helper uses `CMARK_OPT_UNSAFE`, allowing raw HTML.
   Input and rendered strings are freed after use.
3. Apply the template and print the assembled page to stdout.
   `render` reads both paths, converts Markdown, then calls
   `replace(tmpl, PLACEHOLDER, html, result)` and frees its intermediate strings.
   `replace` requires a nonempty needle occurring exactly once (including
   overlapping matches), allocates the result, and leaves its inputs unchanged.
   It reports missing/repeated needles and size overflow, and asserts successful
   allocation.

## Current work

Step 3 is complete using a separate `replace` helper. Only `render` calls `read`.
Title substitution is next.

Current usage, from the repository root:

```sh
make -C typo
typo/bin/typo web/index.html.tmpl content/yacc.md
```

The CLI requires two file paths: `TEMPLATE CONTENT`, both relative to the working
directory unless absolute. From inside `typo/`, run
`bin/typo ../web/index.html.tmpl ../content/yacc.md`.
It prints the template with rendered Markdown inserted at `{{ content }}`.
It does not yet substitute the title, process front matter, or write generated
pages to disk.

Validation performed during these steps: strict C89 compilation with
`-pedantic-errors`, file-reading checks for existing, empty, and larger files,
and Markdown rendering checks for headings, emphasis, empty input, and existing
blog content. Missing files and directory inputs were checked before file-error
handling was changed to assertions. These checks were run before subsequent
user edits to the rendering helper and Makefile.

After adopting integer status returns, strict C89 compilation and checks for
successful rendering, empty input, missing template/content files, directory
input, and missing/repeated placeholders passed. Errors exit with status 1 and
a diagnostic instead of an assertion failure.

After adding the shared reporting helper, strict C89 compilation and checks for
successful rendering, missing files, directory input, and missing/repeated
placeholders passed. Messages include the filename and failed operation where
available.

After introducing `replace`, strict C89 compilation and checks for placeholder
positions at the start, middle, and end, empty content, replacement text that
itself contains the placeholder, and missing/repeated placeholders passed.

## Remaining steps

4. Add a page title: initially proposed first H1 with a filename fallback,
   HTML-escaped when inserted into the template.
5. Write one page to disk, creating directories. Map `content/yacc.md` to
   `public/yacc/index.html`.
6. Walk the content directory and generate every Markdown page. Map
   `content/lex/index.md` to `public/lex/index.html`; reject output collisions.
7. Copy supporting files while preserving relative paths, such as
   `content/lex/lex-diagram.png` to `public/lex/lex-diagram.png`.
8. Finalize the CLI: a bare invocation builds the site using defaults
   `content/`, `web/index.html.tmpl`, and `public/`; add `--content`,
   `--template`, `--output`, and `--help`. Paths are relative to the working
   directory.
9. Make rebuilds reliable: build in a temporary sibling directory, replace
   output after success, remove stale pages, and reject output paths that
   overlap inputs.

Watch mode, feeds, an automatically generated homepage, and incremental builds
are deferred. Front matter handling remains undecided; current content includes
front matter that libcmark presently treats as Markdown.
