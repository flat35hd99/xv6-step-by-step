SRC_DIR := src
DIST_DIR := dist

# Find all .md files recursively in SRC_DIR
SRC_FILES := $(shell find $(SRC_DIR) -type f -name "*.md")
DIST_FILES := $(patsubst $(SRC_DIR)/%.md,$(DIST_DIR)/%.html,$(SRC_FILES))

.PHONY: all clean

all: $(DIST_FILES)

$(DIST_DIR)/%.html: $(SRC_DIR)/%.md
	@mkdir -p $(@D)
	pandoc -s -o $@ $<

clean:
	rm -rf $(DIST_DIR)
