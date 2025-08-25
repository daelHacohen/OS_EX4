# ===== Makefile (Valgrind-only) =====
# Builds server/client and provides Valgrind analysis (memcheck, helgrind, callgrind, cachegrind)
# No code-coverage targets included.

# -------- Toolchain --------
CXX        ?= g++
STD        ?= c++17
WARN       ?= -Wall -Wextra -Wpedantic
OPT        ?= -O0               # O0 recommended for Valgrind accuracy
THREADS    ?= -pthread

CXXFLAGS   ?= -std=$(STD) $(OPT) $(WARN) $(THREADS)
LDFLAGS    ?= $(THREADS)

# -------- Sources --------
SERVER_SRCS := \
    server.cpp \
    objectActive.cpp \
    Algorithms.cpp \
    Graph.cpp \
    UnionFind.cpp

# If you have a client app, keep it. If not, you can delete CLIENT_*.
CLIENT_SRCS := \
    client.cpp

# -------- Targets --------
SERVER_BIN := server
CLIENT_BIN := client

# Objects
SERVER_OBJS := $(SERVER_SRCS:.cpp=.o)
CLIENT_OBJS := $(CLIENT_SRCS:.cpp=.o)

# -------- Default --------
.PHONY: all
all: $(SERVER_BIN) $(CLIENT_BIN)

# -------- Build rules --------
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SERVER_BIN): $(SERVER_OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@

$(CLIENT_BIN): $(CLIENT_OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@

# =======================
# Valgrind Analysis
# =======================
# Usage examples:
#   make memcheck ARGS="..."
#   make helgrind ARGS="..."
#   make callgrind ARGS="..."
#   make cachegrind ARGS="..."
ARGS ?=

# ===== Valgrind targets =====
.PHONY: memcheck helgrind callgrind annotate

memcheck: server
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
	 --num-callers=25 --log-file=valgrind.%p.log ./server

helgrind: server
	valgrind --tool=helgrind --history-level=approx \
	 --log-file=helgrind.%p.log ./server

callgrind: server
	valgrind --tool=callgrind --callgrind-out-file=callgrind.out ./server

annotate: callgrind
	callgrind_annotate callgrind.out | less

# =======================
# Run helpers
# =======================
.PHONY: run-server run-client
run-server: $(SERVER_BIN)
	./$(SERVER_BIN) $(ARGS)

run-client: $(CLIENT_BIN)
	./$(CLIENT_BIN) $(ARGS)

# =======================
# Cleaning
# =======================
.PHONY: clean distclean
clean:
	$(RM) -f $(SERVER_OBJS) $(CLIENT_OBJS) $(SERVER_BIN) $(CLIENT_BIN) \
	       memcheck.log helgrind.log callgrind.txt cachegrind.txt \
	       callgrind.out.* cachegrind.out.*

distclean: clean
	@echo "distclean done"
