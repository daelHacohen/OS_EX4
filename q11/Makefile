# ===== Compiler & flags =====
CXX       := g++
CXXFLAGS  := -Wall -Wextra -O0 -g --coverage -pthread
LDFLAGS   := --coverage -pthread

# ===== Sources =====
SRC_COMMON := Algorithms.cpp Graph.cpp UnionFind.cpp objectActive.cpp
SERVER_SRC := server.cpp $(SRC_COMMON)
CLIENT_SRC := client.cpp $(SRC_COMMON)

OBJS_SERVER := $(SERVER_SRC:.cpp=.o)
OBJS_CLIENT := $(CLIENT_SRC:.cpp=.o)


GCOV_SOURCES := server.cpp client.cpp Algorithms.cpp Graph.cpp UnionFind.cpp objectActive.cpp


RUN_ARGS_SERVER :=
RUN_ARGS_CLIENT :=

# ===== Default targets =====
.PHONY: all clean run run-server run-client coverage coverage-server coverage-client clean_gcov

all: server client

server: $(OBJS_SERVER)
	$(CXX) -o $@ $^ $(LDFLAGS)

client: $(OBJS_CLIENT)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ===== Runs ( *.gcda) =====
run: run-server run-client

run-server: server
	@echo ">>> Running server $(RUN_ARGS_SERVER)"
	-./server $(RUN_ARGS_SERVER)

run-client: client
	@echo ">>> Running client $(RUN_ARGS_CLIENT)"
	-./client $(RUN_ARGS_CLIENT)

# ===== GCOV coverage =====
coverage: clean_gcov all run
	@mkdir -p coverage_gcov
	@echo ">>> Generating gcov reports ONLY for your sources..."
	@for src in $(GCOV_SOURCES); do \
		echo "  - gcov $$src"; \
		gcov -b -r -o . $$src >/dev/null; \
	done
	@mv -f *.gcov coverage_gcov/ 2>/dev/null || true
	@echo ">>> Done. Open coverage_gcov/ for the reports."


coverage-server: clean_gcov server run-server
	@mkdir -p coverage_gcov
	@for src in server.cpp $(SRC_COMMON); do \
		echo "  - gcov $$src"; \
		gcov -b -r -o . $$src >/dev/null; \
	done
	@mv -f *.gcov coverage_gcov/ 2>/dev/null || true
	@echo ">>> Server-only reports: coverage_gcov/"

coverage-client: clean_gcov client run-client
	@mkdir -p coverage_gcov
	@for src in client.cpp $(SRC_COMMON); do \
		echo "  - gcov $$src"; \
		gcov -b -r -o . $$src >/dev/null; \
	done
	@mv -f *.gcov coverage_gcov/ 2>/dev/null || true
	@echo ">>> Client-only reports: coverage_gcov/"

# ===== Cleaning =====
clean_gcov:
	@rm -f *.gcov

clean:
	@rm -f *.o server client *.gcno *.gcda *.gcov
	@rm -rf coverage_gcov
