CC = gcc
CFLAGS = -Wall -Wextra -g
HEADERS = matrix.h vector.h

MAIN_SRC = main.c
APP_NAME = ml_app

TEST_SRC = test.c
TEST_NAME = run_tests

GENERATOR_SRC = generator.c
GENERATOR_NAME = generate

all: $(APP_NAME)

$(APP_NAME): $(MAIN_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $(APP_NAME) $(MAIN_SRC)
	@echo "Main build successful"

test: $(TEST_NAME)

$(TEST_NAME): $(TEST_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TEST_NAME) $(TEST_SRC)
	@echo "Test build successful"

gen: $(GENERATOR_NAME)

$(GENERATOR_NAME): $(GENERATOR_SRC)
	$(CC) $(CFLAGS) -o $(GENERATOR_NAME) $(GENERATOR_SRC)
	@echo "Generator build successful"

clean:
	rm -f $(APP_NAME) $(TEST_NAME) *.o
	@echo "cleaned"

.PHONY: all test clean