NAME := libft.a
SRCS_DIR := ./src/libft
BUILD_DIR := ./build/libft

CC := clang
CFLAGS := -Wall -Wextra -Werror
CFLAGS += -I./include/

SRCS := $(shell find $(SRCS_DIR) -type f -name "*.c")
OBJS := $(SRCS:$(SRCS_DIR)%.c=$(BUILD_DIR)%.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all
all: $(NAME)

-include $(DEPS)

$(BUILD_DIR)%.o: $(SRCS_DIR)%.c Makefile
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -MF $(@:.o=.d) -o $@ -c $<

$(NAME): $(OBJS)
	ar -rcs $(NAME) $(OBJS)

.PHONY: clean
clean:
	$(RM) -r build

.PHONY: fclean
fclean: clean
	$(RM) $(NAME)

.PHONY: re
re: fclean all
