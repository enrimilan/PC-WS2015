#!/bin/sh

SOURCE=doc			# Document name
OUT_DIR=output		# LaTeX output folder


# Create output folder if it does not exist
mkdir -p $OUT_DIR
OPTS=-output-directory=$OUT_DIR
PL_ARGS=$OPTS" "$SOURCE

# Build document
pdflatex $PL_ARGS
bibtex   $OUT_DIR/$SOURCE
pdflatex $PL_ARGS
pdflatex $PL_ARGS

# Move pdf to root folder
mv $OUT_DIR/$SOURCE.pdf $SOURCE.pdf
echo -e "\n\nDocument successfully compiled!"