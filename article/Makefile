.POSIX:
.PHONY: all clean
.SUFFIXES: .md .pdf

MD = article.md
PDF = $(MD:.md=.pdf)

all: $(PDF)

.md.pdf:
	mkdir -p build
	pandoc \
	--toc \
	-s \
	-V colorlinks=true \
	-V toccolor=black \
	-V toc-title="Table of contents" \
	-V linktoc=all \
	$< -o build/$@

clean:
	rm -f build


