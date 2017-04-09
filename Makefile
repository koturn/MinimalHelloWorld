MAKE     := make
DIR_LIST := hello_x64/ gen_hello_x64/

define default-rule
$1:
	@for dir in $(DIR_LIST); do \
		$(MAKE) -C $$$$dir $$@; \
	done
endef


PHONY_TARGETS := test depends asm syntax ctags install uninstall clean distclean
.PHONY: all $(PHONY_TARGETS)

all:
	@for dir in $(DIR_LIST); do \
		$(MAKE) -C $$dir; \
	done

$(foreach TARGET, $(PHONY_TARGETS), $(eval $(call default-rule, $(TARGET))))
