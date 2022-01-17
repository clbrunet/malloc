NAME := libft_malloc.so
SRCS_DIR := ./src
BUILD_DIR := ./build

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif
NAME_HOSTTYPE := $(NAME:.so=_$(HOSTTYPE).so)

LIBFT := ./lib/libft/libft.a

CC := clang
CFLAGS := -Wall -Wextra -Werror
CFLAGS += -I./include/ -I./src/
CFLAGS += -g3
# CFLAGS += -DNDEBUG
LDFLAGS := -L$(dir $(LIBFT))
LDLIBS := -lft

SRCS := $(shell find $(SRCS_DIR) -type f -name "*.c")
OBJS := $(SRCS:$(SRCS_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all
all: $(NAME)

$(LIBFT):
	make -C ./lib/libft/

-include $(DEPS)

$(BUILD_DIR)/%.o: $(SRCS_DIR)/%.c Makefile
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -MF $(@:.o=.d) -o $@ -c $<

$(NAME_HOSTTYPE): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -o $(NAME_HOSTTYPE) $(OBJS) $(LDLIBS)

$(NAME): $(NAME_HOSTTYPE)
	ln -sf $(NAME_HOSTTYPE) $(NAME)

.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)

.PHONY: fclean
fclean: clean
	$(RM) $(NAME_HOSTTYPE) $(NAME)

.PHONY: re
re: fclean all