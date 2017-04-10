include ../common.mk

TARGET := gen_hello_x86
SRCS   := $(addsuffix .c,$(basename $(TARGET)))
OBJS   := $(SRCS:.c=.o)

ifeq ($(OS),Windows_NT)
    TARGET := $(addsuffix .exe,$(TARGET))
endif
INSTALLED_TARGET := $(if $(PREFIX),$(PREFIX),/usr/local)/bin/$(TARGET)


.PHONY: all test depends asm syntax ctags install uninstall clean distclean
all: $(TARGET)
$(TARGET): $(OBJS)

$(foreach SRC,$(SRCS),$(eval $(filter-out \,$(shell $(CC) -MM $(SRC)))))

test: $(TARGET)
	@./$<

depends:
	$(CC) -MM $(SRCS) > $(DEPENDS)

asm:
	$(CC) $(SRC) -S --verbose-asm $(CPPFLAGS)

syntax:
	$(CC) $(SRCS) -fsyntax-only $(CPPFLAGS) $(WARNING_CFLAGS)

ctags:
	$(CTAGS) $(CTAGSFLAGS)

install: $(INSTALLED_TARGET)
$(INSTALLED_TARGET): $(TARGET)
	@[ ! -d $(@D) ] && $(MKDIR) $(@D) || :
	$(CP) $< $@

uninstall:
	$(RM) $(INSTALLED_TARGET)

clean:
	$(RM) $(OBJS)

distclean:
	$(RM) $(TARGET) $(OBJS)
