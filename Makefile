# KERNUL — Phases 0-2 build
#
# Sources: only files in scope for phases 0, 1, and 2.
# Out-of-scope files are not listed here, even if they exist in the tree.
# See docs/ROADMAP.md.
#
# Architecture include path:
#   -Iarch/stub resolves <cpu_state.h> to arch/stub/cpu_state.h.
#   Replace with -Iarch/<target> when building for real hardware.
#   Source files never embed the literal target path. See CODING_STANDARD §15.
#
# Test build: `make test` compiles and runs tests/test_boot.c against
#   the hosted stdlib. The kernel build uses -ffreestanding -nostdlib.

CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -Wpedantic -Werror \
          -ffreestanding -nostdlib                   \
          -Iinclude                                  \
          -Iarch/stub                                \
          -DKERNUL_STUB_TARGET

# Phase 0-4 kernel sources
SRCS = arch/stub/arch.c  \
       core/assert.c     \
       core/address_space.c \
       core/elf_loader.c \
       core/fd_table.c    \
       core/signal.c      \
       core/pipe.c        \
       core/init.c       \
       core/process.c    \
       core/vfs.c        \
       core/syscall_dispatch.c \
       core/spinlock.c   \
       init/boot.c       \
       mm/mm.c           \
       sched/sched.c     \
       sched/idle.c

OBJS = $(SRCS:.c=.o)

# Test build uses hosted stdlib so tests can call exit(), etc.
TEST_CFLAGS = -std=c11 -Wall -Wextra -Werror \
              -Iinclude                        \
              -Iarch/stub                      \
              -DKERNUL_STUB_TARGET

.PHONY: all test clean

all: $(OBJS)
	@echo "Phase 0-2 build: all objects compiled."

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: tests/test_boot tests/test_list tests/test_spinlock tests/test_thread \
       tests/test_runq tests/test_sched_api.o tests/test_sched \
       tests/test_cpu_state_api.o tests/test_idle \
       tests/init/test_init_contract_compile.o \
       tests/init/test_init_link \
       tests/address_space/test_address_space_contract_compile.o \
       tests/address_space/test_address_space_link \
       tests/fd/test_fd_table_contract_compile.o \
       tests/fd/test_fd_table_link \
       tests/signal/test_signal_contract_compile.o \
       tests/signal/test_signal_link \
       tests/vfs/test_vfs_contract_compile.o \
       tests/vfs/test_vfs_link \
       tests/pipe/test_pipe_contract_compile.o \
       tests/pipe/test_pipe_link \
       tests/process/test_process_contract_compile.o \
       tests/process/test_process_link \
       tests/elf/test_elf_loader_contract_compile.o \
       tests/elf/test_elf_loader_link \
       tests/syscall/test_syscall_contract_compile.o \
       tests/syscall/test_sys_dispatch_link \
       tests/arch/x86_64/test_arch_syscall_enter_link \
       tests/arch/aarch64/test_arch_syscall_enter_link
	./tests/test_boot     && echo "test_boot:          passed." || echo "test_boot:          FAILED."
	./tests/test_list     && echo "test_list:          passed." || echo "test_list:          FAILED."
	./tests/test_spinlock && echo "test_spinlock:      passed." || echo "test_spinlock:      FAILED."
	./tests/test_thread   && echo "test_thread:        passed." || echo "test_thread:        FAILED."
	./tests/test_runq     && echo "test_runq:          passed." || echo "test_runq:          FAILED."
	@echo "test_sched_api:      compile-check passed."
	./tests/test_sched    && echo "test_sched:         passed." || echo "test_sched:         FAILED."
	@echo "test_cpu_state_api:  compile-check passed."
	./tests/test_idle     && echo "test_idle:          passed." || echo "test_idle:          FAILED."
	@echo "test_init_contract_compile: compile-check passed."
	./tests/init/test_init_link && echo "test_init_link:     passed." || echo "test_init_link:     FAILED."
	@echo "test_address_space_contract_compile: compile-check passed."
	./tests/address_space/test_address_space_link && echo "test_address_space_link: passed." || echo "test_address_space_link: FAILED."
	@echo "test_fd_table_contract_compile: compile-check passed."
	./tests/fd/test_fd_table_link && echo "test_fd_table_link: passed." || echo "test_fd_table_link: FAILED."
	@echo "test_signal_contract_compile: compile-check passed."
	./tests/signal/test_signal_link && echo "test_signal_link:   passed." || echo "test_signal_link:   FAILED."
	@echo "test_vfs_contract_compile: compile-check passed."
	./tests/vfs/test_vfs_link && echo "test_vfs_link:      passed." || echo "test_vfs_link:      FAILED."
	@echo "test_pipe_contract_compile: compile-check passed."
	./tests/pipe/test_pipe_link && echo "test_pipe_link:     passed." || echo "test_pipe_link:     FAILED."
	@echo "test_process_contract_compile: compile-check passed."
	./tests/process/test_process_link && echo "test_process_link:  passed." || echo "test_process_link:  FAILED."
	@echo "test_elf_loader_contract_compile: compile-check passed."
	./tests/elf/test_elf_loader_link && echo "test_elf_loader_link: passed." || echo "test_elf_loader_link: FAILED."
	@echo "test_syscall_contract_compile: compile-check passed."
	./tests/syscall/test_sys_dispatch_link && echo "test_sys_dispatch_link: passed." || echo "test_sys_dispatch_link: FAILED."
	./tests/arch/x86_64/test_arch_syscall_enter_link && echo "test_arch_syscall_enter_link(x86_64): passed." || echo "test_arch_syscall_enter_link(x86_64): FAILED."
	./tests/arch/aarch64/test_arch_syscall_enter_link && echo "test_arch_syscall_enter_link(aarch64): passed." || echo "test_arch_syscall_enter_link(aarch64): FAILED."

tests/test_boot: tests/test_boot.c arch/stub/arch.c core/assert.c init/boot.c mm/mm.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/test_list: tests/test_list.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/test_spinlock: tests/test_spinlock.c core/spinlock.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/test_thread: tests/test_thread.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/test_runq: tests/test_runq.c core/spinlock.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

# Compile-only: verifies sched.h signature consistency. Linked binary
# is test_sched below.
tests/test_sched_api.o: tests/test_sched_api.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/test_sched: tests/test_sched.c sched/sched.c core/spinlock.c core/assert.c arch/stub/arch.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

# Compile-only: verifies context-switch contract declarations and types.
tests/test_cpu_state_api.o: tests/test_cpu_state_api.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/test_idle: tests/test_idle.c sched/idle.c sched/sched.c \
                 core/spinlock.c core/assert.c arch/stub/arch.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

# Compile-only: verifies init launch contract declarations and signature.
tests/init/test_init_contract_compile.o: tests/init/test_init_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/init/test_init_link: tests/init/test_init_link.c core/init.c \
                           core/process.c core/address_space.c \
                           core/elf_loader.c core/assert.c arch/stub/arch.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

# Compile-only: verifies address-space contract declarations and visible layout.
tests/address_space/test_address_space_contract_compile.o: tests/address_space/test_address_space_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/address_space/test_address_space_link: tests/address_space/test_address_space_link.c core/address_space.c core/assert.c arch/stub/arch.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

# Compile-only: verifies fd-table contract declarations and visible struct layout.
tests/fd/test_fd_table_contract_compile.o: tests/fd/test_fd_table_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/fd/test_fd_table_link: tests/fd/test_fd_table_link.c core/fd_table.c core/assert.c arch/stub/arch.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

# Compile-only: verifies signal contract declarations and visible struct layout.
tests/signal/test_signal_contract_compile.o: tests/signal/test_signal_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/signal/test_signal_link: tests/signal/test_signal_link.c core/signal.c core/assert.c arch/stub/arch.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

# Compile-only: verifies vfs contract declarations and visible struct layout.
tests/vfs/test_vfs_contract_compile.o: tests/vfs/test_vfs_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/vfs/test_vfs_link: tests/vfs/test_vfs_link.c core/vfs.c core/assert.c arch/stub/arch.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

# Compile-only: verifies pipe contract declarations and visible struct layout.
tests/pipe/test_pipe_contract_compile.o: tests/pipe/test_pipe_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/pipe/test_pipe_link: tests/pipe/test_pipe_link.c core/pipe.c core/assert.c arch/stub/arch.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

# Compile-only: verifies process contract declarations and visible struct layout.
tests/process/test_process_contract_compile.o: tests/process/test_process_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/process/test_process_link: tests/process/test_process_link.c core/process.c core/assert.c arch/stub/arch.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

# Compile-only: verifies ELF loader contract declarations and visible struct layout.
tests/elf/test_elf_loader_contract_compile.o: tests/elf/test_elf_loader_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/elf/test_elf_loader_link: tests/elf/test_elf_loader_link.c core/elf_loader.c core/assert.c arch/stub/arch.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

# Compile-only: verifies syscall.h declarations and types.
tests/syscall/test_syscall_contract_compile.o: tests/syscall/test_syscall_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/syscall/test_sys_dispatch_link: tests/syscall/test_sys_dispatch_link.c core/syscall_dispatch.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/arch/x86_64/test_arch_syscall_enter_link: tests/arch/x86_64/test_arch_syscall_enter_link.c \
                                                 arch/x86_64/arch_syscall_enter.c arch/stub/arch.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/arch/aarch64/test_arch_syscall_enter_link: tests/arch/aarch64/test_arch_syscall_enter_link.c \
                                                  arch/aarch64/arch_syscall_enter.c arch/stub/arch.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

clean:
	rm -f $(OBJS) tests/test_boot tests/test_list tests/test_spinlock \
	      tests/test_thread tests/test_runq tests/test_sched_api.o \
	      tests/test_sched tests/test_cpu_state_api.o tests/test_idle \
	      tests/init/test_init_contract_compile.o \
	      tests/init/test_init_link \
	      tests/address_space/test_address_space_contract_compile.o \
	      tests/address_space/test_address_space_link \
	      tests/fd/test_fd_table_contract_compile.o \
	      tests/fd/test_fd_table_link \
	      tests/signal/test_signal_contract_compile.o \
	      tests/signal/test_signal_link \
	      tests/vfs/test_vfs_contract_compile.o \
	      tests/vfs/test_vfs_link \
	      tests/process/test_process_contract_compile.o \
	      tests/process/test_process_link \
	      tests/elf/test_elf_loader_contract_compile.o \
	      tests/elf/test_elf_loader_link \
	      tests/syscall/test_syscall_contract_compile.o \
	      tests/syscall/test_sys_dispatch_link \
	      tests/arch/x86_64/test_arch_syscall_enter_link \
	      tests/arch/aarch64/test_arch_syscall_enter_link
