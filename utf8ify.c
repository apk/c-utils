/* -*- mode: C; c-basic-offset: 8; -*- */

/* run-on-change utf8ify.c test/utf8ify.sh -- sh test/utf8ify.sh
 *
 * Copy stdin or the listed files to stdout, repairing invalid UTF-8.
 * Bytes that don't form a valid UTF-8 sequence are assumed to be
 * ISO-8859-1 (Latin-1), except in the 0x80-0x9F range, which is far
 * more often mis-encoded CP1252; the 5 code points CP1252 leaves
 * undefined there fall back to raw Latin-1 (C1 control codes).
 */

#include "hdr.h"

/* CP1252 mapping for bytes 0x80-0x9f. 0 marks an undefined slot,
 * meaning: fall back to the raw byte value (Latin-1 C1 control code). */
static unsigned int cp1252_tbl [32] = {
	/* 0x80 */ 0x20AC, 0,      0x201A, 0x0192,
	/* 0x84 */ 0x201E, 0x2026, 0x2020, 0x2021,
	/* 0x88 */ 0x02C6, 0x2030, 0x0160, 0x2039,
	/* 0x8c */ 0x0152, 0,      0x017D, 0,
	/* 0x90 */ 0,      0x2018, 0x2019, 0x201C,
	/* 0x94 */ 0x201D, 0x2022, 0x2013, 0x2014,
	/* 0x98 */ 0x02DC, 0x2122, 0x0161, 0x203A,
	/* 0x9c */ 0x0153, 0,      0x017E, 0x0178,
};

/* A byte reader with a small pushback buffer of our own (a portable
 * alternative to ungetc, which the standard only guarantees for a
 * single character). */
struct rdr {
	FILE *f;
	unsigned char pending [3];
	int npending;
};

static void rdr_init (struct rdr *r, FILE *f) {
	r->f = f;
	r->npending = 0;
}

/* Returns a byte value 0-255, or -1 on EOF. */
static int rdr_getc (struct rdr *r) {
	if (r->npending > 0) {
		return r->pending [-- r->npending];
	}
	return getc (r->f);
}

/* Push a byte back so the next rdr_getc () returns it again.
 * Bytes must be pushed back in reverse order of how they were read. */
static void rdr_ungetc (struct rdr *r, unsigned char b) {
	r->pending [r->npending ++] = b;
}

/* Returns the length of the UTF-8 sequence starting with b0 (2, 3, or
 * 4), or 0 if b0 cannot legally start a multi-byte sequence. */
static int seqlen (unsigned char b0) {
	if ((b0 & 0xe0) == 0xc0) {
		if (b0 == 0xc0 || b0 == 0xc1) return 0; /* always overlong */
		return 2;
	}
	if ((b0 & 0xf0) == 0xe0) return 3;
	if ((b0 & 0xf8) == 0xf0) {
		if (b0 > 0xf4) return 0; /* would exceed u+10ffff */
		return 4;
	}
	return 0;
}

/* Write a unicode code point to stdout, utf-8 encoded. */
static void putu8 (unsigned int cp) {
	if (cp < 0x80) {
		putchar (cp);
	} else if (cp < 0x800) {
		putchar (0xc0 | (cp >> 6));
		putchar (0x80 | (cp & 0x3f));
	} else if (cp < 0x10000) {
		putchar (0xe0 | (cp >> 12));
		putchar (0x80 | ((cp >> 6) & 0x3f));
		putchar (0x80 | (cp & 0x3f));
	} else {
		putchar (0xf0 | (cp >> 18));
		putchar (0x80 | ((cp >> 12) & 0x3f));
		putchar (0x80 | ((cp >> 6) & 0x3f));
		putchar (0x80 | (cp & 0x3f));
	}
}

/* Emit a single invalid byte b0 as Latin-1/CP1252, as appropriate. */
static void fallback (unsigned char b0) {
	unsigned int cp;
	if (b0 >= 0x80 && b0 <= 0x9f) {
		cp = cp1252_tbl [b0 - 0x80];
		if (cp == 0) cp = b0; /* undefined cp1252 slot -> latin-1 */
	} else {
		cp = b0; /* latin-1: byte value == code point */
	}
	putu8 (cp);
}

/* Attempt to read and strictly validate a complete UTF-8 sequence of
 * 'len' bytes starting with b0. On success, writes the raw bytes
 * through unchanged and returns 1. On failure, pushes back whatever
 * was consumed beyond b0 so it gets reprocessed fresh, and returns 0. */
static int tryseq (struct rdr *r, unsigned char b0, int len) {
	unsigned char buf [4];
	int i, got, valid;
	unsigned int cp;

	buf [0] = b0;
	got = 0;
	for (i = 1; i < len; i ++) {
		int c = rdr_getc (r);
		if (c == -1) break;
		if ((c & 0xc0) != 0x80) {
			rdr_ungetc (r, c);
			break;
		}
		buf [i] = c;
		got ++;
	}

	if (got != len - 1) {
		/* Incomplete sequence: push back whatever continuation
		 * bytes we did consume, so they're reprocessed one by one. */
		for (i = got; i >= 1; i --) rdr_ungetc (r, buf [i]);
		return 0;
	}

	switch (len) {
	case 2:
		cp = (b0 & 0x1f) << 6
		   | (buf [1] & 0x3f);
		break;
	case 3:
		cp = (b0 & 0x0f) << 12
		   | (buf [1] & 0x3f) << 6
		   | (buf [2] & 0x3f);
		break;
	default:
		cp = (b0 & 0x07) << 18
		   | (buf [1] & 0x3f) << 12
		   | (buf [2] & 0x3f) << 6
		   | (buf [3] & 0x3f);
		break;
	}

	valid = 1;
	if (len == 3 && cp < 0x800) valid = 0;      /* overlong */
	if (len == 4 && cp < 0x10000) valid = 0;    /* overlong */
	if (cp >= 0xd800 && cp <= 0xdfff) valid = 0; /* surrogate half */
	if (cp > 0x10ffff) valid = 0;               /* out of range */

	if (!valid) {
		for (i = len - 1; i >= 1; i --) rdr_ungetc (r, buf [i]);
		return 0;
	}

	fwrite (buf, 1, len, stdout);
	return 1;
}

static void dofd (FILE *f) {
	struct rdr r;
	int c;

	rdr_init (&r, f);
	while ((c = rdr_getc (&r)) != -1) {
		unsigned char b0 = c;
		int len;

		if (b0 < 0x80) {
			putchar (b0);
			continue;
		}

		len = seqlen (b0);
		if (len != 0 && tryseq (&r, b0, len)) {
			continue;
		}

		fallback (b0);
	}
}

int main (int argc, char **argv) {
	int i, status = 0;

	if (argc <= 1) {
		dofd (stdin);
		return 0;
	}

	for (i = 1; i < argc; i ++) {
		char *name = argv [i];
		FILE *f;

		if (!strcmp (name, "-")) {
			dofd (stdin);
			continue;
		}
		f = fopen (name, "rb");
		if (!f) {
			perror (name);
			status = 1;
			continue;
		}
		dofd (f);
		fclose (f);
	}

	return status;
}
