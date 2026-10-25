CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2
INCLUDES ?= -I src

# GTK & Cairo & SQLite flags for App and Adapters
PKG_PACKAGES := gtk+-3.0 cairo sqlite3
PKG_CFLAGS   := $(shell pkg-config --cflags $(PKG_PACKAGES) 2>/dev/null)
PKG_LIBS     := $(shell pkg-config --libs $(PKG_PACKAGES) 2>/dev/null)

# Sanitizer flags for memory leak checking
ASAN_FLAGS := -fsanitize=address,undefined -g -O1

BIN_DIR := bin
SRC_DIR := src
TESTS_DIR := tests

# Default target
.PHONY: all test test_domain test_db test_e2e test_asan clean directories

all: directories $(BIN_DIR)/gach test

directories:
	@mkdir -p $(BIN_DIR)

# --- TẦNG DOMAIN TESTS (Pure C++17, Zero IO, Zero DB, Zero GTK, < 15ms) ---
test_domain: directories
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(TESTS_DIR)/unit/test_phonics_syllable.cpp $(SRC_DIR)/domain/phonics_syllable.cpp -o $(BIN_DIR)/test_phonics_syllable
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(TESTS_DIR)/unit/test_flow_fsm.cpp $(SRC_DIR)/domain/flow_fsm.cpp -o $(BIN_DIR)/test_flow_fsm
	@echo "=== RUNNING DOMAIN UNIT TESTS ==="
	@$(BIN_DIR)/test_phonics_syllable
	@$(BIN_DIR)/test_flow_fsm
	@echo "Domain Unit Tests: ALL PASSED!"

# --- TẦNG INTEGRATION TESTS (SQLite3 in-memory & Use Cases) ---
test_db: directories
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(TESTS_DIR)/integration/test_sqlite_repository.cpp 		$(SRC_DIR)/domain/phonics_syllable.cpp 		$(SRC_DIR)/domain/flow_fsm.cpp 		$(SRC_DIR)/adapters/db/sqlite_story_repository.cpp 		-lsqlite3 -o $(BIN_DIR)/test_sqlite_repository
	@echo "=== RUNNING SQLITE REPOSITORY INTEGRATION TESTS ==="
	@$(BIN_DIR)/test_sqlite_repository
	@echo "SQLite Integration Tests: ALL PASSED!"

# --- END-TO-END FLOW TESTS ---
test_e2e: directories
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(TESTS_DIR)/e2e/test_story_flow_e2e.cpp \
		$(SRC_DIR)/domain/phonics_syllable.cpp \
		$(SRC_DIR)/domain/flow_fsm.cpp \
		$(SRC_DIR)/adapters/ui/ui_layout.cpp \
		$(SRC_DIR)/application/story_interactor.cpp \
		$(SRC_DIR)/adapters/db/sqlite_story_repository.cpp \
		-lsqlite3 -o $(BIN_DIR)/test_story_flow_e2e
	@echo "=== RUNNING E2E WORKFLOW TESTS ==="
	@$(BIN_DIR)/test_story_flow_e2e
	@echo "E2E Tests: ALL PASSED!"

# --- TOÀN BỘ TESTS ---
test: test_domain test_db test_e2e

# --- KIỂM TRA RÒ RỈ BỘ NHỚ VỚI ADDRESS SANITIZER ---
test_asan: directories
	$(CXX) $(CXXFLAGS) $(ASAN_FLAGS) $(INCLUDES) $(TESTS_DIR)/unit/test_phonics_syllable.cpp $(SRC_DIR)/domain/phonics_syllable.cpp -o $(BIN_DIR)/test_phonics_asan
	$(CXX) $(CXXFLAGS) $(ASAN_FLAGS) $(INCLUDES) $(TESTS_DIR)/unit/test_flow_fsm.cpp $(SRC_DIR)/domain/flow_fsm.cpp -o $(BIN_DIR)/test_fsm_asan
	$(CXX) $(CXXFLAGS) $(ASAN_FLAGS) $(INCLUDES) $(TESTS_DIR)/integration/test_sqlite_repository.cpp \
		$(SRC_DIR)/domain/phonics_syllable.cpp \
		$(SRC_DIR)/domain/flow_fsm.cpp \
		$(SRC_DIR)/adapters/db/sqlite_story_repository.cpp \
		-lsqlite3 -o $(BIN_DIR)/test_db_asan
	$(CXX) $(CXXFLAGS) $(ASAN_FLAGS) $(INCLUDES) $(TESTS_DIR)/e2e/test_story_flow_e2e.cpp \
		$(SRC_DIR)/domain/phonics_syllable.cpp \
		$(SRC_DIR)/domain/flow_fsm.cpp \
		$(SRC_DIR)/adapters/ui/ui_layout.cpp \
		$(SRC_DIR)/application/story_interactor.cpp \
		$(SRC_DIR)/adapters/db/sqlite_story_repository.cpp \
		-lsqlite3 -o $(BIN_DIR)/test_e2e_asan
	@echo "=== RUNNING ASAN MEMORY & UB AUDIT ACROSS ALL TIERS ==="
	@$(BIN_DIR)/test_phonics_asan
	@$(BIN_DIR)/test_fsm_asan
	@$(BIN_DIR)/test_db_asan
	@$(BIN_DIR)/test_e2e_asan
	@echo "Memory & Undefined Behavior Audit: 0 LEAKS, 0 ERRORS ACROSS ALL TESTS!"

# --- BUILD ỨNG DỤNG HOÀN CHỈNH (HOST GTK3/CAIRO) ---
$(BIN_DIR)/gach: directories
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(PKG_CFLAGS) 		$(SRC_DIR)/main.cpp 		$(SRC_DIR)/domain/phonics_syllable.cpp 		$(SRC_DIR)/domain/flow_fsm.cpp 		$(SRC_DIR)/application/story_interactor.cpp 		$(SRC_DIR)/adapters/db/sqlite_story_repository.cpp 		$(SRC_DIR)/adapters/ui/cairo_renderer.cpp 		$(SRC_DIR)/adapters/ui/ui_layout.cpp 		$(SRC_DIR)/adapters/ui/gtk_touch_adapter.cpp 		$(SRC_DIR)/infrastructure/kindle_power.cpp 		$(SRC_DIR)/infrastructure/app_config.cpp 		$(PKG_LIBS) -o $(BIN_DIR)/gach

clean:
	rm -rf $(BIN_DIR)
