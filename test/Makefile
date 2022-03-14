NAME := test
SRCS_DIR := ./src
BUILD_DIR := ./build

LIBFT_MALLOC := ../libft_malloc.so

CC := clang
CFLAGS := -Wall -Wextra -Werror
CFLAGS += -I../include/ -I./src/
# CFLAGS += -g3
CFLAGS += -DNDEBUG
LDFLAGS := -L../
LDLIBS := -lpthread
LDLIBS += -lft_malloc

SRCS := $(shell find $(SRCS_DIR) -type f -name "*.c")
OBJS := $(SRCS:$(SRCS_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all
all: $(NAME)

-include $(DEPS)

$(BUILD_DIR)/%.o: $(SRCS_DIR)/%.c Makefile
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -MF $(@:.o=.d) -o $@ -c $<

$(LIBFT_MALLOC):
	make -C $(dir $@)

$(NAME): $(LIBFT_MALLOC) $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(NAME) $(OBJS) $(LDLIBS)

.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)

.PHONY: fclean
fclean: clean
	$(RM) $(NAME)

.PHONY: re
re: fclean all
