# KERNUL — Pre-Alpha build
#
# Sources: kernel-internal structural and stub surfaces currently in scope
# through the completed roadmap phases implemented in the repository.
# See docs/ROADMAP.md.
#
# Architecture include path:
#   -Iarch/stub resolves <cpu_state.h> to arch/stub/cpu_state.h.
#   Replace with -Iarch/<target> when building for real hardware.
#   Source files never embed the literal target path.
#
# Test build: `make test` compiles and runs hosted test binaries where
# appropriate. Compile-contract checks are object-only and are not executed.
# The kernel object build uses -ffreestanding -nostdlib.

CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -Wpedantic -Werror \
          -ffreestanding -nostdlib                   \
          -Iinclude                                  \
          -Iarch/stub                                \
          -DKERNUL_STUB_TARGET

TEST_CFLAGS = -std=c11 -Wall -Wextra -Werror \
              -Iinclude                        \
              -Iarch/stub                      \
              -DKERNUL_STUB_TARGET

SRCS = arch/stub/arch.c              \
        core/assert.c                 \
        core/address_space.c          \
        core/controlling_terminal.c   \
        core/elf_loader.c             \
        core/fd_table.c               \
        core/init.c                   \
        core/interactive_scheduler_container.c \
        core/interactive_scheduler_state.c \
        core/interactive_admission.c  \
        core/interactive_activation.c \
        core/interactive_console.c    \
        core/interactive_dispatch.c   \
        core/interactive_selection.c  \
        core/interactive_selection_handoff_binding.c \
        core/interactive_execution_handoff.c \
        core/interactive_execution_transfer_operands_view.c \
        core/interactive_execution_transfer_initiation_gate.c \
        core/interactive_execution_outcome_record.c \
        core/interactive_execution_outcome_view.c \
        core/interactive_execution_outcome_aggregate.c \
        core/interactive_execution_failure_ack_gate.c \
        core/interactive_execution_failure_initiation_gate.c \
        core/interactive_execution_completion_ack_gate.c \
        core/interactive_execution_completion_initiation_gate.c \
        core/session_terminal_state.c \
        core/session_finalization.c \
        core/session_terminal_cause.c \
        core/session_ownership.c \
        core/session_reclamation.c \
        core/session_recovery_eligibility.c \
        core/session_recovery_eligibility_storage.c \
        core/session_recovery_authorization.c \
        core/session_recovery_authorization_storage.c \
        core/session_recovery_execution.c \
        core/session_recovery_execution_storage.c \
        core/session_recovery_outcome.c \
        core/session_recovery_outcome_storage.c \
        core/session_recovery_outcome_observe.c \
        core/session_retry_policy.c \
        core/session_retry_policy_storage.c \
        core/session_retry_authorization.c \
        core/session_retry_authorization_storage.c \
        core/session_retry_execution.c \
        core/session_retry_execution_result.c \
        core/session_retry_outcome.c \
        core/session_retry_outcome_observation.c \
        core/interactive_execution_target.c \
        core/interactive_execution.c  \
        core/interactive_switch_operands.c \
        core/interactive_runnable.c   \
        core/interactive_readiness.c  \
        core/pipe.c                   \
        core/process.c                \
        core/session.c                \
       core/signal.c                 \
       core/spinlock.c               \
       core/syscall_dispatch.c       \
       core/terminal.c               \
       core/vfs.c                    \
       init/boot.c                   \
       mm/mm.c                       \
       sched/idle.c                  \
       sched/sched.c

OBJS = $(SRCS:.c=.o)

.PHONY: all test clean test_sched_api.o test_cpu_state_api.o \
        test_init_contract_compile.o \
        test_interactive_scheduler_container_contract_compile.o \
        test_interactive_scheduler_state_contract_compile.o \
        test_interactive_admission_contract_compile.o \
        test_address_space_contract_compile.o \
        test_interactive_activation_contract_compile.o \
        test_interactive_console_contract_compile.o \
        test_interactive_dispatch_contract_compile.o \
        test_interactive_selection_contract_compile.o \
        test_interactive_selection_handoff_binding_contract_compile.o \
        test_interactive_execution_handoff_contract_compile.o \
        test_interactive_execution_transfer_operands_view_contract_compile.o \
        test_interactive_execution_transfer_initiation_gate_contract_compile.o \
        test_interactive_execution_outcome_record_contract_compile.o \
        test_interactive_execution_outcome_view_contract_compile.o \
        test_interactive_execution_outcome_aggregate_contract_compile.o \
        test_interactive_execution_failure_ack_gate_contract_compile.o \
        test_interactive_execution_failure_initiation_gate_contract_compile.o \
        test_interactive_execution_completion_ack_gate_contract_compile.o \
        test_interactive_execution_completion_initiation_gate_contract_compile.o \
        test_session_terminal_state_contract_compile.o \
        test_session_finalization_contract_compile.o \
        test_session_terminal_cause_contract_compile.o \
        test_session_ownership_contract_compile.o \
        test_session_reclamation_contract_compile.o \
        test_session_reclamation_include_ban.o \
        test_session_recovery_eligibility_contract_compile.o \
        test_session_recovery_eligibility_include_ban.o \
        test_session_recovery_authorization_contract_compile.o \
        test_session_recovery_authorization_include_ban.o \
        test_session_recovery_execution_contract_compile.o \
        test_session_recovery_execution_include_ban.o \
        test_session_recovery_execution_storage_include_ban.o \
        test_session_recovery_outcome_contract_compile.o \
        test_session_recovery_outcome_include_ban.o \
        test_session_recovery_outcome_storage_include_ban.o \
        test_session_recovery_outcome_observe_contract_compile.o \
        test_session_recovery_outcome_observe_include_ban.o \
        test_session_retry_policy_contract_compile.o \
        test_session_retry_policy_include_ban.o \
        test_session_retry_policy_storage_include_ban.o \
        test_session_retry_authorization_contract_compile.o \
        test_session_retry_authorization_include_ban.o \
        test_session_retry_authorization_storage_include_ban.o \
         test_session_retry_execution_contract_compile.o \
         test_session_retry_execution_include_ban.o \
         test_session_retry_execution_result_contract_compile.o \
         test_session_retry_execution_result_include_ban.o \
         test_session_retry_outcome_contract_compile.o \
         test_session_retry_outcome_include_ban.o \
         test_session_retry_outcome_observation_contract_compile.o \
         test_session_retry_outcome_observation_include_ban.o \
         test_interactive_execution_target_contract_compile.o \
        test_interactive_execution_contract_compile.o \
        test_interactive_switch_operands_contract_compile.o \
        test_interactive_runnable_contract_compile.o \
        test_interactive_readiness_contract_compile.o \
        test_fd_table_contract_compile.o \
        test_signal_contract_compile.o \
        test_session_contract_compile.o \
        test_vfs_contract_compile.o \
        test_pipe_contract_compile.o \
        test_terminal_contract_compile.o \
        test_controlling_terminal_contract_compile.o \
        test_process_contract_compile.o \
        test_elf_loader_contract_compile.o \
        test_syscall_contract_compile.o

all: $(OBJS)
	@echo "KERNUL build: all objects compiled."

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: tests/test_boot tests/test_list tests/test_spinlock tests/test_thread \
      tests/test_runq tests/test_sched_api.o tests/test_sched \
       tests/test_cpu_state_api.o tests/test_idle \
       tests/init/test_init_contract_compile.o \
        tests/init/test_init_link \
           tests/console/test_interactive_scheduler_container_contract_compile.o \
           tests/console/test_interactive_scheduler_container_link \
           tests/address_space/test_address_space_contract_compile.o \
            tests/address_space/test_address_space_link \
            tests/console/test_interactive_scheduler_state_contract_compile.o \
           tests/console/test_interactive_scheduler_state_link \
           tests/console/test_interactive_admission_contract_compile.o \
           tests/console/test_interactive_admission_link \
           tests/console/test_interactive_activation_contract_compile.o \
            tests/console/test_interactive_activation_link \
             tests/console/test_interactive_console_contract_compile.o \
             tests/console/test_interactive_console_link \
               tests/console/test_interactive_dispatch_contract_compile.o \
               tests/console/test_interactive_dispatch_link \
                tests/console/test_interactive_selection_contract_compile.o \
                tests/console/test_interactive_selection_link \
                tests/console/test_interactive_selection_handoff_binding_contract_compile.o \
                 tests/console/test_interactive_selection_handoff_binding_link \
                 tests/console/test_interactive_execution_handoff_contract_compile.o \
                 tests/console/test_interactive_execution_handoff_link \
                 tests/console/test_interactive_execution_transfer_operands_view_contract_compile.o \
                 tests/console/test_interactive_execution_transfer_operands_view_link \
                   tests/console/test_interactive_execution_transfer_initiation_gate_contract_compile.o \
                   tests/console/test_interactive_execution_transfer_initiation_gate_link \
                    tests/console/test_interactive_execution_outcome_record_contract_compile.o \
                     tests/console/test_interactive_execution_outcome_record_link \
                     tests/console/test_interactive_execution_outcome_view_contract_compile.o \
                     tests/console/test_interactive_execution_outcome_view_link \
                      tests/console/test_interactive_execution_outcome_aggregate_contract_compile.o \
                      tests/console/test_interactive_execution_outcome_aggregate_link \
                       tests/console/test_interactive_execution_failure_ack_gate_contract_compile.o \
                       tests/console/test_interactive_execution_failure_ack_gate_link \
                       tests/console/test_interactive_execution_failure_initiation_gate_contract_compile.o \
                       tests/console/test_interactive_execution_failure_initiation_gate_link \
                       tests/console/test_interactive_execution_completion_ack_gate_contract_compile.o \
                       tests/console/test_interactive_execution_completion_ack_gate_link \
                        tests/console/test_interactive_execution_completion_initiation_gate_contract_compile.o \
                         tests/console/test_interactive_execution_completion_initiation_gate_link \
                         tests/console/test_session_terminal_state_contract_compile.o \
                         tests/console/test_session_terminal_state_link \
                         tests/console/test_session_finalization_contract_compile.o \
                         tests/console/test_session_finalization_link \
                         tests/console/test_session_terminal_cause_contract_compile.o \
                         tests/console/test_session_terminal_cause_link \
                          tests/console/test_session_ownership_contract_compile.o \
                          tests/console/test_session_ownership_link \
                          tests/console/test_session_reclamation_contract_compile.o \
                          tests/console/test_session_reclamation_include_ban.o \
                          tests/console/test_session_reclamation_link \
                           tests/console/test_session_recovery_eligibility_contract_compile.o \
                           tests/console/test_session_recovery_eligibility_include_ban.o \
                           tests/console/test_session_recovery_eligibility_link \
                           tests/console/test_session_recovery_authorization_contract_compile.o \
                           tests/console/test_session_recovery_authorization_include_ban.o \
                           tests/console/test_session_recovery_authorization_link \
                            tests/console/test_session_recovery_execution_contract_compile.o \
                            tests/console/test_session_recovery_execution_include_ban.o \
                            tests/console/test_session_recovery_execution_storage_include_ban.o \
                            tests/console/test_session_recovery_execution_link \
                             tests/console/test_session_recovery_outcome_contract_compile.o \
                             tests/console/test_session_recovery_outcome_include_ban.o \
                             tests/console/test_session_recovery_outcome_storage_include_ban.o \
                             tests/console/test_session_recovery_outcome_link \
                             tests/console/test_session_recovery_outcome_observe_contract_compile.o \
                             tests/console/test_session_recovery_outcome_observe_include_ban.o \
                             tests/console/test_session_recovery_outcome_observe_link \
                             tests/console/test_session_retry_policy_contract_compile.o \
                             tests/console/test_session_retry_policy_include_ban.o \
                             tests/console/test_session_retry_policy_storage_include_ban.o \
                             tests/console/test_session_retry_policy_link \
                             tests/console/test_session_retry_authorization_contract_compile.o \
                             tests/console/test_session_retry_authorization_include_ban.o \
                             tests/console/test_session_retry_authorization_storage_include_ban.o \
                              tests/console/test_session_retry_authorization_link \
                               tests/console/test_session_retry_execution_contract_compile.o \
                               tests/console/test_session_retry_execution_include_ban.o \
                               tests/console/test_session_retry_execution_link \
                               tests/console/test_session_retry_execution_result_contract_compile.o \
                               tests/console/test_session_retry_execution_result_include_ban.o \
                                tests/console/test_session_retry_execution_result_link \
                                tests/console/test_session_retry_outcome_contract_compile.o \
                                tests/console/test_session_retry_outcome_include_ban.o \
                                tests/console/test_session_retry_outcome_link \
                                tests/console/test_session_retry_outcome_observation_contract_compile.o \
                                tests/console/test_session_retry_outcome_observation_include_ban.o \
                                tests/console/test_session_retry_outcome_observation_link \
                                  tests/console/test_interactive_execution_target_contract_compile.o \
                tests/console/test_interactive_execution_target_link \
              tests/console/test_interactive_execution_contract_compile.o \
             tests/console/test_interactive_execution_link \
             tests/console/test_interactive_switch_operands_contract_compile.o \
             tests/console/test_interactive_switch_operands_link \
             tests/console/test_interactive_runnable_contract_compile.o \
             tests/console/test_interactive_runnable_link \
             tests/console/test_interactive_readiness_contract_compile.o \
            tests/console/test_interactive_readiness_link \
         tests/fd/test_fd_table_contract_compile.o \
        tests/fd/test_fd_table_link \
       tests/signal/test_signal_contract_compile.o \
       tests/signal/test_signal_link \
	      tests/session/test_session_contract_compile.o \
	      tests/session/test_session_link \
	      tests/session/test_session_readiness_link \
	      tests/vfs/test_vfs_contract_compile.o \
      tests/vfs/test_vfs_link \
      tests/pipe/test_pipe_contract_compile.o \
      tests/pipe/test_pipe_link \
      tests/terminal/test_terminal_contract_compile.o \
      tests/terminal/test_terminal_link \
      tests/terminal/test_controlling_terminal_contract_compile.o \
      tests/terminal/test_controlling_terminal_link \
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
	@echo "test_interactive_scheduler_container_contract_compile: compile-check passed."
	./tests/console/test_interactive_scheduler_container_link && echo "test_interactive_scheduler_container_link: passed." || echo "test_interactive_scheduler_container_link: FAILED."
	@echo "test_address_space_contract_compile: compile-check passed."
	./tests/address_space/test_address_space_link && echo "test_address_space_link: passed." || echo "test_address_space_link: FAILED."
	@echo "test_interactive_scheduler_state_contract_compile: compile-check passed."
	./tests/console/test_interactive_scheduler_state_link && echo "test_interactive_scheduler_state_link: passed." || echo "test_interactive_scheduler_state_link: FAILED."
	@echo "test_interactive_admission_contract_compile: compile-check passed."
	./tests/console/test_interactive_admission_link && echo "test_interactive_admission_link: passed." || echo "test_interactive_admission_link: FAILED."
	@echo "test_interactive_activation_contract_compile: compile-check passed."
	./tests/console/test_interactive_activation_link && echo "test_interactive_activation_link: passed." || echo "test_interactive_activation_link: FAILED."
	@echo "test_interactive_console_contract_compile: compile-check passed."
	./tests/console/test_interactive_console_link && echo "test_interactive_console_link: passed." || echo "test_interactive_console_link: FAILED."
	@echo "test_interactive_dispatch_contract_compile: compile-check passed."
	./tests/console/test_interactive_dispatch_link && echo "test_interactive_dispatch_link: passed." || echo "test_interactive_dispatch_link: FAILED."
	@echo "test_interactive_selection_contract_compile: compile-check passed."
	./tests/console/test_interactive_selection_link && echo "test_interactive_selection_link: passed." || echo "test_interactive_selection_link: FAILED."
	@echo "test_interactive_selection_handoff_binding_contract_compile: compile-check passed."
	./tests/console/test_interactive_selection_handoff_binding_link && echo "test_interactive_selection_handoff_binding_link: passed." || echo "test_interactive_selection_handoff_binding_link: FAILED."
	@echo "test_interactive_execution_handoff_contract_compile: compile-check passed."
	./tests/console/test_interactive_execution_handoff_link && echo "test_interactive_execution_handoff_link: passed." || echo "test_interactive_execution_handoff_link: FAILED."
	@echo "test_interactive_execution_transfer_operands_view_contract_compile: compile-check passed."
	./tests/console/test_interactive_execution_transfer_operands_view_link && echo "test_interactive_execution_transfer_operands_view_link: passed." || echo "test_interactive_execution_transfer_operands_view_link: FAILED."
	@echo "test_interactive_execution_transfer_initiation_gate_contract_compile: compile-check passed."
	./tests/console/test_interactive_execution_transfer_initiation_gate_link && echo "test_interactive_execution_transfer_initiation_gate_link: passed." || echo "test_interactive_execution_transfer_initiation_gate_link: FAILED."
	@echo "test_interactive_execution_outcome_record_contract_compile: compile-check passed."
	./tests/console/test_interactive_execution_outcome_record_link && echo "test_interactive_execution_outcome_record_link: passed." || echo "test_interactive_execution_outcome_record_link: FAILED."
	@echo "test_interactive_execution_outcome_view_contract_compile: compile-check passed."
	./tests/console/test_interactive_execution_outcome_view_link && echo "test_interactive_execution_outcome_view_link: passed." || echo "test_interactive_execution_outcome_view_link: FAILED."
	@echo "test_interactive_execution_outcome_aggregate_contract_compile: compile-check passed."
	./tests/console/test_interactive_execution_outcome_aggregate_link && echo "test_interactive_execution_outcome_aggregate_link: passed." || echo "test_interactive_execution_outcome_aggregate_link: FAILED."
	@echo "test_interactive_execution_failure_ack_gate_contract_compile: compile-check passed."
	./tests/console/test_interactive_execution_failure_ack_gate_link && echo "test_interactive_execution_failure_ack_gate_link: passed." || echo "test_interactive_execution_failure_ack_gate_link: FAILED."
	@echo "test_interactive_execution_failure_initiation_gate_contract_compile: compile-check passed."
	./tests/console/test_interactive_execution_failure_initiation_gate_link && echo "test_interactive_execution_failure_initiation_gate_link: passed." || echo "test_interactive_execution_failure_initiation_gate_link: FAILED."
	@echo "test_interactive_execution_completion_ack_gate_contract_compile: compile-check passed."
	./tests/console/test_interactive_execution_completion_ack_gate_link && echo "test_interactive_execution_completion_ack_gate_link: passed." || echo "test_interactive_execution_completion_ack_gate_link: FAILED."
	@echo "test_interactive_execution_completion_initiation_gate_contract_compile: compile-check passed."
	./tests/console/test_interactive_execution_completion_initiation_gate_link && echo "test_interactive_execution_completion_initiation_gate_link: passed." || echo "test_interactive_execution_completion_initiation_gate_link: FAILED."
	@echo "test_session_terminal_state_contract_compile: compile-check passed."
	./tests/console/test_session_terminal_state_link && echo "test_session_terminal_state_link: passed." || echo "test_session_terminal_state_link: FAILED."
	@echo "test_session_finalization_contract_compile: compile-check passed."
	./tests/console/test_session_finalization_link && echo "test_session_finalization_link: passed." || echo "test_session_finalization_link: FAILED."
	@echo "test_session_terminal_cause_contract_compile: compile-check passed."
	./tests/console/test_session_terminal_cause_link && echo "test_session_terminal_cause_link: passed." || echo "test_session_terminal_cause_link: FAILED."
	@echo "test_session_ownership_contract_compile: compile-check passed."
	./tests/console/test_session_ownership_link && echo "test_session_ownership_link: passed." || echo "test_session_ownership_link: FAILED."
	@echo "test_session_reclamation_contract_compile: compile-check passed."
	@echo "test_session_reclamation_include_ban: compile-check passed."
	./tests/console/test_session_reclamation_link && echo "test_session_reclamation_link: passed." || echo "test_session_reclamation_link: FAILED."
	@echo "test_session_recovery_eligibility_contract_compile: compile-check passed."
	@echo "test_session_recovery_eligibility_include_ban: compile-check passed."
	./tests/console/test_session_recovery_eligibility_link && echo "test_session_recovery_eligibility_link: passed." || echo "test_session_recovery_eligibility_link: FAILED."
	@echo "test_session_recovery_authorization_contract_compile: compile-check passed."
	@echo "test_session_recovery_authorization_include_ban: compile-check passed."
	./tests/console/test_session_recovery_authorization_link && echo "test_session_recovery_authorization_link: passed." || echo "test_session_recovery_authorization_link: FAILED."
	@echo "test_session_recovery_execution_contract_compile: compile-check passed."
	@echo "test_session_recovery_execution_include_ban: compile-check passed."
	@echo "test_session_recovery_execution_storage_include_ban: compile-check passed."
	./tests/console/test_session_recovery_execution_link && echo "test_session_recovery_execution_link: passed." || echo "test_session_recovery_execution_link: FAILED."
	@echo "test_session_recovery_outcome_contract_compile: compile-check passed."
	@echo "test_session_recovery_outcome_include_ban: compile-check passed."
	@echo "test_session_recovery_outcome_storage_include_ban: compile-check passed."
	./tests/console/test_session_recovery_outcome_link && echo "test_session_recovery_outcome_link: passed." || echo "test_session_recovery_outcome_link: FAILED."
	@echo "test_session_recovery_outcome_observe_contract_compile: compile-check passed."
	@echo "test_session_recovery_outcome_observe_include_ban: compile-check passed."
	./tests/console/test_session_recovery_outcome_observe_link && echo "test_session_recovery_outcome_observe_link: passed." || echo "test_session_recovery_outcome_observe_link: FAILED."
	@echo "test_session_retry_policy_contract_compile: compile-check passed."
	@echo "test_session_retry_policy_include_ban: compile-check passed."
	@echo "test_session_retry_policy_storage_include_ban: compile-check passed."
	./tests/console/test_session_retry_policy_link && echo "test_session_retry_policy_link: passed." || echo "test_session_retry_policy_link: FAILED."
	@echo "test_session_retry_authorization_contract_compile: compile-check passed."
	@echo "test_session_retry_authorization_include_ban: compile-check passed."
	@echo "test_session_retry_authorization_storage_include_ban: compile-check passed."
	./tests/console/test_session_retry_authorization_link && echo "test_session_retry_authorization_link: passed." || echo "test_session_retry_authorization_link: FAILED."
	@echo "test_session_retry_execution_contract_compile: compile-check passed."
	@echo "test_session_retry_execution_include_ban: compile-check passed."
	./tests/console/test_session_retry_execution_link && echo "test_session_retry_execution_link: passed." || echo "test_session_retry_execution_link: FAILED."
	@echo "test_session_retry_execution_result_contract_compile: compile-check passed."
	@echo "test_session_retry_execution_result_include_ban: compile-check passed."
	./tests/console/test_session_retry_execution_result_link && echo "test_session_retry_execution_result_link: passed." || echo "test_session_retry_execution_result_link: FAILED."
	@echo "test_session_retry_outcome_contract_compile: compile-check passed."
	@echo "test_session_retry_outcome_include_ban: compile-check passed."
	./tests/console/test_session_retry_outcome_link && echo "test_session_retry_outcome_link: passed." || echo "test_session_retry_outcome_link: FAILED."
	@echo "test_session_retry_outcome_observation_contract_compile: compile-check passed."
	@echo "test_session_retry_outcome_observation_include_ban: compile-check passed."
	./tests/console/test_session_retry_outcome_observation_link && echo "test_session_retry_outcome_observation_link: passed." || echo "test_session_retry_outcome_observation_link: FAILED."
	@echo "test_session_contract_compile: compile-check passed."
	./tests/session/test_session_link && echo "test_session_link:  passed." || echo "test_session_link:  FAILED."
	./tests/session/test_session_readiness_link && echo "test_session_readiness_link: passed." || echo "test_session_readiness_link: FAILED."
	@echo "test_interactive_execution_target_contract_compile: compile-check passed."
	./tests/console/test_interactive_execution_target_link && echo "test_interactive_execution_target_link: passed." || echo "test_interactive_execution_target_link: FAILED."
	@echo "test_interactive_execution_contract_compile: compile-check passed."
	./tests/console/test_interactive_execution_link && echo "test_interactive_execution_link: passed." || echo "test_interactive_execution_link: FAILED."
	@echo "test_interactive_switch_operands_contract_compile: compile-check passed."
	./tests/console/test_interactive_switch_operands_link && echo "test_interactive_switch_operands_link: passed." || echo "test_interactive_switch_operands_link: FAILED."
	@echo "test_interactive_runnable_contract_compile: compile-check passed."
	./tests/console/test_interactive_runnable_link && echo "test_interactive_runnable_link: passed." || echo "test_interactive_runnable_link: FAILED."
	@echo "test_interactive_readiness_contract_compile: compile-check passed."
	./tests/console/test_interactive_readiness_link && echo "test_interactive_readiness_link: passed." || echo "test_interactive_readiness_link: FAILED."
	@echo "test_fd_table_contract_compile: compile-check passed."
	./tests/fd/test_fd_table_link && echo "test_fd_table_link: passed." || echo "test_fd_table_link: FAILED."
	@echo "test_signal_contract_compile: compile-check passed."
	./tests/signal/test_signal_link && echo "test_signal_link:   passed." || echo "test_signal_link:   FAILED."
	@echo "test_session_contract_compile: compile-check passed."
	./tests/session/test_session_link && echo "test_session_link:  passed." || echo "test_session_link:  FAILED."
	@echo "test_vfs_contract_compile: compile-check passed."
	./tests/vfs/test_vfs_link && echo "test_vfs_link:      passed." || echo "test_vfs_link:      FAILED."
	@echo "test_pipe_contract_compile: compile-check passed."
	./tests/pipe/test_pipe_link && echo "test_pipe_link:     passed." || echo "test_pipe_link:     FAILED."
	@echo "test_terminal_contract_compile: compile-check passed."
	./tests/terminal/test_terminal_link && echo "test_terminal_link: passed." || echo "test_terminal_link: FAILED."
	@echo "test_controlling_terminal_contract_compile: compile-check passed."
	./tests/terminal/test_controlling_terminal_link && echo "test_controlling_terminal_link: passed." || echo "test_controlling_terminal_link: FAILED."
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

tests/test_sched_api.o: tests/test_sched_api.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/test_sched: tests/test_sched.c sched/sched.c core/spinlock.c core/assert.c arch/stub/arch.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/test_cpu_state_api.o: tests/test_cpu_state_api.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/test_idle: tests/test_idle.c sched/idle.c sched/sched.c core/spinlock.c core/assert.c arch/stub/arch.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/init/test_init_contract_compile.o: tests/init/test_init_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/init/test_init_link: tests/init/test_init_link.c core/init.c \
                           core/process.c core/address_space.c \
                           core/elf_loader.c core/assert.c arch/stub/arch.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/address_space/test_address_space_contract_compile.o: tests/address_space/test_address_space_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/address_space/test_address_space_link: tests/address_space/test_address_space_link.c core/address_space.c core/assert.c arch/stub/arch.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_interactive_activation_contract_compile.o: tests/console/test_interactive_activation_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_interactive_scheduler_container_contract_compile.o: tests/console/test_interactive_scheduler_container_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_interactive_scheduler_container_link: tests/console/test_interactive_scheduler_container_link.c core/interactive_scheduler_container.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_interactive_scheduler_state_contract_compile.o: tests/console/test_interactive_scheduler_state_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_interactive_scheduler_state_link: tests/console/test_interactive_scheduler_state_link.c core/interactive_scheduler_state.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_interactive_admission_contract_compile.o: tests/console/test_interactive_admission_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_interactive_admission_link: tests/console/test_interactive_admission_link.c core/interactive_admission.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_interactive_activation_link: tests/console/test_interactive_activation_link.c core/interactive_activation.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_interactive_console_contract_compile.o: tests/console/test_interactive_console_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_interactive_console_link: tests/console/test_interactive_console_link.c core/interactive_console.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_interactive_dispatch_contract_compile.o: tests/console/test_interactive_dispatch_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_interactive_dispatch_link: tests/console/test_interactive_dispatch_link.c core/interactive_dispatch.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_interactive_selection_contract_compile.o: tests/console/test_interactive_selection_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_interactive_selection_link: tests/console/test_interactive_selection_link.c \
                                               core/interactive_selection.c \
                                               core/interactive_scheduler_state.c \
                                               core/interactive_scheduler_container.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_interactive_selection_handoff_binding_contract_compile.o: tests/console/test_interactive_selection_handoff_binding_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_interactive_selection_handoff_binding_link: tests/console/test_interactive_selection_handoff_binding_link.c \
                                                               core/interactive_selection_handoff_binding.c \
                                                               core/interactive_selection.c \
                                                               core/interactive_execution_handoff.c \
                                                               core/interactive_scheduler_state.c \
                                                               core/interactive_scheduler_container.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_interactive_execution_handoff_contract_compile.o: tests/console/test_interactive_execution_handoff_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_interactive_execution_handoff_link: tests/console/test_interactive_execution_handoff_link.c core/interactive_execution_handoff.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_interactive_execution_transfer_operands_view_contract_compile.o: tests/console/test_interactive_execution_transfer_operands_view_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_interactive_execution_transfer_operands_view_link: tests/console/test_interactive_execution_transfer_operands_view_link.c \
                                                                       core/interactive_execution_transfer_operands_view.c \
                                                                       core/interactive_execution_handoff.c \
                                                                       core/session.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_interactive_execution_transfer_initiation_gate_contract_compile.o: tests/console/test_interactive_execution_transfer_initiation_gate_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_interactive_execution_transfer_initiation_gate_link: tests/console/test_interactive_execution_transfer_initiation_gate_link.c \
                                                                           core/interactive_execution_transfer_initiation_gate.c \
                                                                           core/interactive_execution_failure_initiation_gate.c \
                                                                           core/interactive_execution_failure_ack_gate.c \
                                                                           core/interactive_execution_transfer_operands_view.c \
                                                                           core/interactive_execution_handoff.c \
                                                                           core/interactive_execution_outcome_view.c \
                                                                           core/interactive_execution.c \
                                                                           core/session.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_interactive_execution_outcome_record_contract_compile.o: tests/console/test_interactive_execution_outcome_record_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_interactive_execution_outcome_record_link: tests/console/test_interactive_execution_outcome_record_link.c \
                                                               core/interactive_execution_outcome_record.c \
                                                               core/interactive_execution.c \
                                                               core/session.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_interactive_execution_outcome_view_contract_compile.o: tests/console/test_interactive_execution_outcome_view_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_interactive_execution_outcome_view_link: tests/console/test_interactive_execution_outcome_view_link.c \
                                                              core/interactive_execution_outcome_view.c \
                                                              core/session.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_interactive_execution_outcome_aggregate_contract_compile.o: tests/console/test_interactive_execution_outcome_aggregate_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_interactive_execution_outcome_aggregate_link: tests/console/test_interactive_execution_outcome_aggregate_link.c \
                                                                  core/interactive_execution_outcome_aggregate.c \
                                                                  core/interactive_execution_outcome_view.c \
                                                                  core/session.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_interactive_execution_failure_ack_gate_contract_compile.o: tests/console/test_interactive_execution_failure_ack_gate_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_interactive_execution_failure_ack_gate_link: tests/console/test_interactive_execution_failure_ack_gate_link.c \
                                                                 core/interactive_execution_failure_ack_gate.c \
                                                                 core/interactive_execution_outcome_view.c \
                                                                 core/session.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_interactive_execution_failure_initiation_gate_contract_compile.o: tests/console/test_interactive_execution_failure_initiation_gate_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_interactive_execution_failure_initiation_gate_link: tests/console/test_interactive_execution_failure_initiation_gate_link.c \
                                                                        core/interactive_execution_failure_initiation_gate.c \
                                                                        core/interactive_execution_failure_ack_gate.c \
                                                                        core/interactive_execution_transfer_initiation_gate.c \
                                                                        core/interactive_execution_transfer_operands_view.c \
                                                                        core/interactive_execution_handoff.c \
                                                                        core/interactive_execution.c \
                                                                        core/interactive_execution_outcome_view.c \
                                                                        core/session.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_interactive_execution_completion_ack_gate_contract_compile.o: tests/console/test_interactive_execution_completion_ack_gate_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_interactive_execution_completion_ack_gate_link: tests/console/test_interactive_execution_completion_ack_gate_link.c \
                                                                    core/interactive_execution_completion_ack_gate.c \
                                                                    core/interactive_execution_outcome_view.c \
                                                                    core/session.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_interactive_execution_completion_initiation_gate_contract_compile.o: tests/console/test_interactive_execution_completion_initiation_gate_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_interactive_execution_completion_initiation_gate_link: tests/console/test_interactive_execution_completion_initiation_gate_link.c \
                                                                           core/interactive_execution_completion_initiation_gate.c \
                                                                           core/interactive_execution_completion_ack_gate.c \
                                                                           core/interactive_execution_transfer_initiation_gate.c \
                                                                           core/interactive_execution_failure_initiation_gate.c \
                                                                           core/interactive_execution_failure_ack_gate.c \
                                                                           core/interactive_execution_transfer_operands_view.c \
                                                                           core/interactive_execution_handoff.c \
                                                                           core/interactive_execution_outcome_view.c \
                                                                           core/interactive_execution.c \
                                                                           core/session.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_session_terminal_state_contract_compile.o: tests/console/test_session_terminal_state_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_terminal_state_link: tests/console/test_session_terminal_state_link.c \
                                                   core/session_terminal_state.c \
                                                   core/interactive_execution_failure_initiation_gate.c \
                                                   core/interactive_execution_failure_ack_gate.c \
                                                   core/interactive_execution_outcome_view.c \
                                                   core/session.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_session_finalization_contract_compile.o: tests/console/test_session_finalization_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_finalization_link: tests/console/test_session_finalization_link.c \
                                              core/session_finalization.c \
                                              core/session_terminal_state.c \
                                              core/interactive_execution_failure_initiation_gate.c \
                                              core/interactive_execution_failure_ack_gate.c \
                                              core/interactive_execution_outcome_view.c \
                                              core/session.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_session_terminal_cause_contract_compile.o: tests/console/test_session_terminal_cause_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_terminal_cause_link: tests/console/test_session_terminal_cause_link.c \
                                                 core/session_terminal_cause.c \
                                                 core/session_terminal_state.c \
                                                 core/interactive_execution_failure_initiation_gate.c \
                                                 core/interactive_execution_failure_ack_gate.c \
                                                 core/interactive_execution_completion_ack_gate.c \
                                                 core/interactive_execution_outcome_view.c \
                                                 core/session.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_session_ownership_contract_compile.o: tests/console/test_session_ownership_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_ownership_link: tests/console/test_session_ownership_link.c \
                                           core/session_ownership.c \
                                           core/session.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_session_reclamation_contract_compile.o: tests/console/test_session_reclamation_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_reclamation_include_ban.o: tests/console/test_session_reclamation_include_ban.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_reclamation_link: tests/console/test_session_reclamation_link.c \
                                              core/session_reclamation.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_session_recovery_eligibility_contract_compile.o: tests/console/test_session_recovery_eligibility_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_recovery_eligibility_include_ban.o: tests/console/test_session_recovery_eligibility_include_ban.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_recovery_eligibility_link: tests/console/test_session_recovery_eligibility_link.c \
                                                       core/session_recovery_eligibility.c \
                                                       core/session_recovery_eligibility_storage.c \
                                                       core/session.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_session_recovery_authorization_contract_compile.o: tests/console/test_session_recovery_authorization_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_recovery_authorization_include_ban.o: tests/console/test_session_recovery_authorization_include_ban.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_recovery_authorization_link: tests/console/test_session_recovery_authorization_link.c \
                                                         core/session_recovery_authorization.c \
                                                         core/session_recovery_authorization_storage.c \
                                                         core/session.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_session_recovery_execution_contract_compile.o: tests/console/test_session_recovery_execution_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_recovery_execution_include_ban.o: tests/console/test_session_recovery_execution_include_ban.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_recovery_execution_storage_include_ban.o: tests/console/test_session_recovery_execution_storage_include_ban.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_recovery_execution_link: tests/console/test_session_recovery_execution_link.c \
                                                    core/session_recovery_execution.c \
                                                    core/session_recovery_execution_storage.c \
                                                    core/session.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_session_recovery_outcome_contract_compile.o: tests/console/test_session_recovery_outcome_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_recovery_outcome_include_ban.o: tests/console/test_session_recovery_outcome_include_ban.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_recovery_outcome_storage_include_ban.o: tests/console/test_session_recovery_outcome_storage_include_ban.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_recovery_outcome_link: tests/console/test_session_recovery_outcome_link.c \
                                                   core/session_recovery_outcome.c \
                                                   core/session_recovery_outcome_storage.c \
                                                   core/session_recovery_execution_storage.c \
                                                   core/session.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_session_recovery_outcome_observe_contract_compile.o: tests/console/test_session_recovery_outcome_observe_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_recovery_outcome_observe_include_ban.o: tests/console/test_session_recovery_outcome_observe_include_ban.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_recovery_outcome_observe_link: tests/console/test_session_recovery_outcome_observe_link.c \
                                                           core/session_recovery_outcome_observe.c \
                                                           core/session_recovery_outcome_storage.c \
                                                           core/session_recovery_execution_storage.c \
                                                           core/session.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_session_retry_policy_contract_compile.o: tests/console/test_session_retry_policy_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_retry_policy_include_ban.o: tests/console/test_session_retry_policy_include_ban.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_retry_policy_storage_include_ban.o: tests/console/test_session_retry_policy_storage_include_ban.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_retry_policy_link: tests/console/test_session_retry_policy_link.c \
                                              core/session_retry_policy.c \
                                              core/session_retry_policy_storage.c \
                                              core/session.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_session_retry_authorization_contract_compile.o: tests/console/test_session_retry_authorization_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_retry_authorization_include_ban.o: tests/console/test_session_retry_authorization_include_ban.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_retry_authorization_storage_include_ban.o: tests/console/test_session_retry_authorization_storage_include_ban.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_retry_authorization_link: tests/console/test_session_retry_authorization_link.c \
                                                      core/session_retry_authorization.c \
                                                      core/session_retry_authorization_storage.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_session_retry_execution_contract_compile.o: tests/console/test_session_retry_execution_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_retry_execution_include_ban.o: tests/console/test_session_retry_execution_include_ban.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_retry_execution_link: tests/console/test_session_retry_execution_link.c \
                                                 core/session_retry_execution.c \
                                                 core/session_retry_authorization_storage.c \
                                                 core/session_retry_execution_result.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_session_retry_execution_result_contract_compile.o: tests/console/test_session_retry_execution_result_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_retry_execution_result_include_ban.o: tests/console/test_session_retry_execution_result_include_ban.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_retry_execution_result_link: tests/console/test_session_retry_execution_result_link.c \
                                                        core/session_retry_execution_result.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_session_retry_outcome_contract_compile.o: tests/console/test_session_retry_outcome_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_retry_outcome_include_ban.o: tests/console/test_session_retry_outcome_include_ban.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_retry_outcome_link: tests/console/test_session_retry_outcome_link.c \
                                                core/session_retry_outcome.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_session_retry_outcome_observation_contract_compile.o: tests/console/test_session_retry_outcome_observation_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_retry_outcome_observation_include_ban.o: tests/console/test_session_retry_outcome_observation_include_ban.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_session_retry_outcome_observation_link: tests/console/test_session_retry_outcome_observation_link.c \
                                                            core/session_retry_outcome_observation.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_interactive_execution_target_contract_compile.o: tests/console/test_interactive_execution_target_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_interactive_execution_target_link: tests/console/test_interactive_execution_target_link.c core/interactive_execution_target.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_interactive_execution_contract_compile.o: tests/console/test_interactive_execution_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_interactive_execution_link: tests/console/test_interactive_execution_link.c core/interactive_execution.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_interactive_switch_operands_contract_compile.o: tests/console/test_interactive_switch_operands_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_interactive_switch_operands_link: tests/console/test_interactive_switch_operands_link.c core/interactive_switch_operands.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_interactive_runnable_contract_compile.o: tests/console/test_interactive_runnable_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_interactive_runnable_link: tests/console/test_interactive_runnable_link.c core/interactive_runnable.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/console/test_interactive_readiness_contract_compile.o: tests/console/test_interactive_readiness_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/console/test_interactive_readiness_link: tests/console/test_interactive_readiness_link.c core/interactive_readiness.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/fd/test_fd_table_contract_compile.o: tests/fd/test_fd_table_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/fd/test_fd_table_link: tests/fd/test_fd_table_link.c core/fd_table.c core/assert.c arch/stub/arch.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/signal/test_signal_contract_compile.o: tests/signal/test_signal_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/signal/test_signal_link: tests/signal/test_signal_link.c core/signal.c core/assert.c arch/stub/arch.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/session/test_session_contract_compile.o: tests/session/test_session_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/session/test_session_link: tests/session/test_session_link.c core/session.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/session/test_session_readiness_link: tests/session/test_session_readiness_link.c core/session.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/vfs/test_vfs_contract_compile.o: tests/vfs/test_vfs_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/vfs/test_vfs_link: tests/vfs/test_vfs_link.c core/vfs.c core/assert.c arch/stub/arch.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/pipe/test_pipe_contract_compile.o: tests/pipe/test_pipe_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/pipe/test_pipe_link: tests/pipe/test_pipe_link.c core/pipe.c core/assert.c arch/stub/arch.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/terminal/test_terminal_contract_compile.o: tests/terminal/test_terminal_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/terminal/test_terminal_link: tests/terminal/test_terminal_link.c core/terminal.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/terminal/test_controlling_terminal_contract_compile.o: tests/terminal/test_controlling_terminal_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/terminal/test_controlling_terminal_link: tests/terminal/test_controlling_terminal_link.c core/controlling_terminal.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/process/test_process_contract_compile.o: tests/process/test_process_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/process/test_process_link: tests/process/test_process_link.c core/process.c core/assert.c arch/stub/arch.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

tests/elf/test_elf_loader_contract_compile.o: tests/elf/test_elf_loader_contract_compile.c
	$(CC) $(TEST_CFLAGS) -c $< -o $@

tests/elf/test_elf_loader_link: tests/elf/test_elf_loader_link.c core/elf_loader.c core/assert.c arch/stub/arch.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

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

# Compatibility aliases for CI callers that invoke short compile-check targets.
test_sched_api.o: tests/test_sched_api.o
test_cpu_state_api.o: tests/test_cpu_state_api.o
test_init_contract_compile.o: tests/init/test_init_contract_compile.o
test_interactive_scheduler_container_contract_compile.o: tests/console/test_interactive_scheduler_container_contract_compile.o
test_interactive_scheduler_state_contract_compile.o: tests/console/test_interactive_scheduler_state_contract_compile.o
test_interactive_admission_contract_compile.o: tests/console/test_interactive_admission_contract_compile.o
test_address_space_contract_compile.o: tests/address_space/test_address_space_contract_compile.o
test_interactive_activation_contract_compile.o: tests/console/test_interactive_activation_contract_compile.o
test_interactive_console_contract_compile.o: tests/console/test_interactive_console_contract_compile.o
test_interactive_dispatch_contract_compile.o: tests/console/test_interactive_dispatch_contract_compile.o
test_interactive_selection_contract_compile.o: tests/console/test_interactive_selection_contract_compile.o
test_interactive_selection_handoff_binding_contract_compile.o: tests/console/test_interactive_selection_handoff_binding_contract_compile.o
test_interactive_execution_transfer_operands_view_contract_compile.o: tests/console/test_interactive_execution_transfer_operands_view_contract_compile.o
test_interactive_execution_transfer_initiation_gate_contract_compile.o: tests/console/test_interactive_execution_transfer_initiation_gate_contract_compile.o
test_interactive_execution_outcome_record_contract_compile.o: tests/console/test_interactive_execution_outcome_record_contract_compile.o
test_interactive_execution_outcome_view_contract_compile.o: tests/console/test_interactive_execution_outcome_view_contract_compile.o
test_interactive_execution_outcome_aggregate_contract_compile.o: tests/console/test_interactive_execution_outcome_aggregate_contract_compile.o
test_interactive_execution_failure_ack_gate_contract_compile.o: tests/console/test_interactive_execution_failure_ack_gate_contract_compile.o
test_interactive_execution_failure_initiation_gate_contract_compile.o: tests/console/test_interactive_execution_failure_initiation_gate_contract_compile.o
test_interactive_execution_completion_ack_gate_contract_compile.o: tests/console/test_interactive_execution_completion_ack_gate_contract_compile.o
test_interactive_execution_completion_initiation_gate_contract_compile.o: tests/console/test_interactive_execution_completion_initiation_gate_contract_compile.o
test_session_terminal_state_contract_compile.o: tests/console/test_session_terminal_state_contract_compile.o
test_session_finalization_contract_compile.o: tests/console/test_session_finalization_contract_compile.o
test_session_terminal_cause_contract_compile.o: tests/console/test_session_terminal_cause_contract_compile.o
test_session_ownership_contract_compile.o: tests/console/test_session_ownership_contract_compile.o
test_session_reclamation_contract_compile.o: tests/console/test_session_reclamation_contract_compile.o
test_session_reclamation_include_ban.o: tests/console/test_session_reclamation_include_ban.o
test_session_recovery_eligibility_contract_compile.o: tests/console/test_session_recovery_eligibility_contract_compile.o
test_session_recovery_eligibility_include_ban.o: tests/console/test_session_recovery_eligibility_include_ban.o
test_session_recovery_authorization_contract_compile.o: tests/console/test_session_recovery_authorization_contract_compile.o
test_session_recovery_authorization_include_ban.o: tests/console/test_session_recovery_authorization_include_ban.o
test_session_recovery_execution_contract_compile.o: tests/console/test_session_recovery_execution_contract_compile.o
test_session_recovery_execution_include_ban.o: tests/console/test_session_recovery_execution_include_ban.o
test_session_recovery_execution_storage_include_ban.o: tests/console/test_session_recovery_execution_storage_include_ban.o
test_session_recovery_outcome_contract_compile.o: tests/console/test_session_recovery_outcome_contract_compile.o
test_session_recovery_outcome_include_ban.o: tests/console/test_session_recovery_outcome_include_ban.o
test_session_recovery_outcome_storage_include_ban.o: tests/console/test_session_recovery_outcome_storage_include_ban.o
test_session_recovery_outcome_observe_contract_compile.o: tests/console/test_session_recovery_outcome_observe_contract_compile.o
test_session_recovery_outcome_observe_include_ban.o: tests/console/test_session_recovery_outcome_observe_include_ban.o
test_session_retry_policy_contract_compile.o: tests/console/test_session_retry_policy_contract_compile.o
test_session_retry_policy_include_ban.o: tests/console/test_session_retry_policy_include_ban.o
test_session_retry_policy_storage_include_ban.o: tests/console/test_session_retry_policy_storage_include_ban.o
test_session_retry_authorization_contract_compile.o: tests/console/test_session_retry_authorization_contract_compile.o
test_session_retry_authorization_include_ban.o: tests/console/test_session_retry_authorization_include_ban.o
test_session_retry_authorization_storage_include_ban.o: tests/console/test_session_retry_authorization_storage_include_ban.o
test_session_retry_execution_contract_compile.o: tests/console/test_session_retry_execution_contract_compile.o
test_session_retry_execution_include_ban.o: tests/console/test_session_retry_execution_include_ban.o
test_session_retry_execution_result_contract_compile.o: tests/console/test_session_retry_execution_result_contract_compile.o
test_session_retry_execution_result_include_ban.o: tests/console/test_session_retry_execution_result_include_ban.o
test_session_retry_outcome_contract_compile.o: tests/console/test_session_retry_outcome_contract_compile.o
test_session_retry_outcome_include_ban.o: tests/console/test_session_retry_outcome_include_ban.o
test_session_retry_outcome_observation_contract_compile.o: tests/console/test_session_retry_outcome_observation_contract_compile.o
test_session_retry_outcome_observation_include_ban.o: tests/console/test_session_retry_outcome_observation_include_ban.o
test_interactive_execution_target_contract_compile.o: tests/console/test_interactive_execution_target_contract_compile.o
test_interactive_execution_contract_compile.o: tests/console/test_interactive_execution_contract_compile.o
test_interactive_switch_operands_contract_compile.o: tests/console/test_interactive_switch_operands_contract_compile.o
test_interactive_runnable_contract_compile.o: tests/console/test_interactive_runnable_contract_compile.o
test_interactive_readiness_contract_compile.o: tests/console/test_interactive_readiness_contract_compile.o
test_fd_table_contract_compile.o: tests/fd/test_fd_table_contract_compile.o
test_signal_contract_compile.o: tests/signal/test_signal_contract_compile.o
test_session_contract_compile.o: tests/session/test_session_contract_compile.o
test_vfs_contract_compile.o: tests/vfs/test_vfs_contract_compile.o
test_pipe_contract_compile.o: tests/pipe/test_pipe_contract_compile.o
test_terminal_contract_compile.o: tests/terminal/test_terminal_contract_compile.o
test_controlling_terminal_contract_compile.o: tests/terminal/test_controlling_terminal_contract_compile.o
test_process_contract_compile.o: tests/process/test_process_contract_compile.o
test_elf_loader_contract_compile.o: tests/elf/test_elf_loader_contract_compile.o
test_syscall_contract_compile.o: tests/syscall/test_syscall_contract_compile.o

clean:
	rm -f $(OBJS) \
	      tests/test_boot tests/test_list tests/test_spinlock \
	      tests/test_thread tests/test_runq tests/test_sched_api.o \
	      tests/test_sched tests/test_cpu_state_api.o tests/test_idle \
	      tests/init/test_init_contract_compile.o \
	      tests/init/test_init_link \
	      tests/console/test_interactive_scheduler_container_contract_compile.o \
	      tests/console/test_interactive_scheduler_container_link \
	      tests/address_space/test_address_space_contract_compile.o \
	      tests/address_space/test_address_space_link \
	      tests/console/test_interactive_scheduler_state_contract_compile.o \
	      tests/console/test_interactive_scheduler_state_link \
	      tests/console/test_interactive_admission_contract_compile.o \
	      tests/console/test_interactive_admission_link \
	      tests/console/test_interactive_activation_contract_compile.o \
	      tests/console/test_interactive_activation_link \
	      tests/console/test_interactive_console_contract_compile.o \
	      tests/console/test_interactive_console_link \
	      tests/console/test_interactive_dispatch_contract_compile.o \
	      tests/console/test_interactive_dispatch_link \
	      tests/console/test_interactive_selection_contract_compile.o \
	      tests/console/test_interactive_selection_link \
	      tests/console/test_interactive_selection_handoff_binding_contract_compile.o \
	      tests/console/test_interactive_selection_handoff_binding_link \
	      tests/console/test_interactive_execution_handoff_contract_compile.o \
	      tests/console/test_interactive_execution_handoff_link \
	      tests/console/test_interactive_execution_transfer_operands_view_contract_compile.o \
	      tests/console/test_interactive_execution_transfer_operands_view_link \
	      tests/console/test_interactive_execution_transfer_initiation_gate_contract_compile.o \
	      tests/console/test_interactive_execution_transfer_initiation_gate_link \
	      tests/console/test_interactive_execution_outcome_record_contract_compile.o \
	      tests/console/test_interactive_execution_outcome_record_link \
	      tests/console/test_interactive_execution_outcome_view_contract_compile.o \
	      tests/console/test_interactive_execution_outcome_view_link \
	      tests/console/test_interactive_execution_outcome_aggregate_contract_compile.o \
	      tests/console/test_interactive_execution_outcome_aggregate_link \
	      tests/console/test_interactive_execution_failure_ack_gate_contract_compile.o \
	      tests/console/test_interactive_execution_failure_ack_gate_link \
	      tests/console/test_interactive_execution_failure_initiation_gate_contract_compile.o \
	      tests/console/test_interactive_execution_failure_initiation_gate_link \
	      tests/console/test_interactive_execution_completion_ack_gate_contract_compile.o \
	      tests/console/test_interactive_execution_completion_ack_gate_link \
	      tests/console/test_interactive_execution_completion_initiation_gate_contract_compile.o \
	      tests/console/test_interactive_execution_completion_initiation_gate_link \
	      tests/console/test_session_terminal_state_contract_compile.o \
	      tests/console/test_session_terminal_state_link \
	      tests/console/test_session_finalization_contract_compile.o \
	      tests/console/test_session_finalization_link \
	      tests/console/test_session_terminal_cause_contract_compile.o \
	      tests/console/test_session_terminal_cause_link \
	      tests/console/test_session_ownership_contract_compile.o \
	      tests/console/test_session_ownership_link \
	      tests/console/test_session_reclamation_contract_compile.o \
	      tests/console/test_session_reclamation_include_ban.o \
	      tests/console/test_session_reclamation_link \
	      tests/console/test_session_recovery_eligibility_contract_compile.o \
	      tests/console/test_session_recovery_eligibility_include_ban.o \
	      tests/console/test_session_recovery_eligibility_link \
	      tests/console/test_session_recovery_authorization_contract_compile.o \
	      tests/console/test_session_recovery_authorization_include_ban.o \
	      tests/console/test_session_recovery_authorization_link \
	      tests/console/test_session_recovery_execution_contract_compile.o \
	      tests/console/test_session_recovery_execution_include_ban.o \
	      tests/console/test_session_recovery_execution_storage_include_ban.o \
	      tests/console/test_session_recovery_execution_link \
	      tests/console/test_session_recovery_outcome_contract_compile.o \
	      tests/console/test_session_recovery_outcome_include_ban.o \
	      tests/console/test_session_recovery_outcome_storage_include_ban.o \
	      tests/console/test_session_recovery_outcome_link \
	      tests/console/test_session_recovery_outcome_observe_contract_compile.o \
	      tests/console/test_session_recovery_outcome_observe_include_ban.o \
	      tests/console/test_session_recovery_outcome_observe_link \
	      tests/console/test_session_retry_policy_contract_compile.o \
	      tests/console/test_session_retry_policy_include_ban.o \
	      tests/console/test_session_retry_policy_storage_include_ban.o \
	      tests/console/test_session_retry_policy_link \
	      tests/console/test_session_retry_authorization_contract_compile.o \
	      tests/console/test_session_retry_authorization_include_ban.o \
	      tests/console/test_session_retry_authorization_storage_include_ban.o \
	      tests/console/test_session_retry_authorization_link \
	      tests/console/test_session_retry_execution_contract_compile.o \
	      tests/console/test_session_retry_execution_include_ban.o \
	      tests/console/test_session_retry_execution_link \
	      tests/console/test_session_retry_execution_result_contract_compile.o \
	      tests/console/test_session_retry_execution_result_include_ban.o \
	      tests/console/test_session_retry_execution_result_link \
	      tests/console/test_session_retry_outcome_contract_compile.o \
	      tests/console/test_session_retry_outcome_include_ban.o \
	      tests/console/test_session_retry_outcome_link \
	      tests/console/test_session_retry_outcome_observation_contract_compile.o \
	      tests/console/test_session_retry_outcome_observation_include_ban.o \
	      tests/console/test_session_retry_outcome_observation_link \
	      tests/console/test_interactive_execution_target_contract_compile.o \
	      tests/console/test_interactive_execution_target_link \
	      tests/console/test_interactive_execution_contract_compile.o \
	      tests/console/test_interactive_execution_link \
	      tests/console/test_interactive_switch_operands_contract_compile.o \
	      tests/console/test_interactive_switch_operands_link \
	      tests/console/test_interactive_runnable_contract_compile.o \
	      tests/console/test_interactive_runnable_link \
	      tests/console/test_interactive_readiness_contract_compile.o \
	      tests/console/test_interactive_readiness_link \
	      tests/fd/test_fd_table_contract_compile.o \
	      tests/fd/test_fd_table_link \
	      tests/signal/test_signal_contract_compile.o \
	      tests/signal/test_signal_link \
	      tests/session/test_session_contract_compile.o \
	      tests/session/test_session_link \
	      tests/session/test_session_readiness_link \
	      tests/vfs/test_vfs_contract_compile.o \
	      tests/vfs/test_vfs_link \
	      tests/pipe/test_pipe_contract_compile.o \
	      tests/pipe/test_pipe_link \
	      tests/terminal/test_terminal_contract_compile.o \
	      tests/terminal/test_terminal_link \
	      tests/terminal/test_controlling_terminal_contract_compile.o \
	      tests/terminal/test_controlling_terminal_link \
	      tests/process/test_process_contract_compile.o \
	      tests/process/test_process_link \
	      tests/elf/test_elf_loader_contract_compile.o \
	      tests/elf/test_elf_loader_link \
	      tests/syscall/test_syscall_contract_compile.o \
	      tests/syscall/test_sys_dispatch_link \
	      tests/arch/x86_64/test_arch_syscall_enter_link \
	      tests/arch/aarch64/test_arch_syscall_enter_link
