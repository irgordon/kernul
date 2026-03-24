# AGENTS.md — Agent Execution Rules

Status: Binding  
Scope: All agent-generated code  
Purpose: Keep implementation deterministic, reviewable, and phase-correct

---

# Core Principle

One task.  
One surface.

Do not expand scope.  
Do not redesign architecture.  
Do not anticipate future phases.

Implement only what the task explicitly requires.

---

# Mandatory Workflow

Before making any change, read and obey:

- ARCHITECTURE.md
- ROADMAP.md
- REPOSITORY_RULES.md
- CODING_STANDARD.md
- COMMENTING_GUIDE.md

These documents define authority.

If a rule conflicts with your assumptions, the rule wins.

---

# Task Execution Model

Every task must:

- implement exactly one surface
- satisfy the declared contract
- compile cleanly
- keep the repository buildable
- keep the repository reviewable

A surface is:

- one public header
- one contract definition
- one subsystem entry point
- one architecture boundary

A task may include supporting implementation files for that surface.

A task must not:

- modify unrelated subsystems
- introduce helpers not required by the task
- add policy decisions
- change existing contracts without instruction

---

# Phase Discipline

Development proceeds in phases.

Do not implement behavior that belongs to a future phase.

Examples:

- do not add scheduling policy before scheduler structure exists
- do not perform context switching before the switch contract exists
- do not introduce preemption before the scheduler supports it
- do not assume a valid current thread before the bootstrap boundary

Violations must be rejected.

---

# Deterministic Stop Condition

At the end of every task, the repository must remain:

- buildable
- testable
- deterministic
- mechanically reviewable
- scope-bounded

If any of these conditions fail, the task is incomplete.

---

# Implementation Rules

Follow these rules exactly.

## 1. Define shape before behavior

Create structures first.

Then implement minimal behavior.

Do not introduce policy.

---

## 2. Use minimal behavior

A stub may:

- initialize state
- enforce assertions
- return fixed values
- maintain structural invariants

A stub must not:

- implement scheduling policy
- implement performance optimization
- introduce concurrency assumptions
- depend on future features

---

## 3. Respect ownership and locking

Every mutable field must document:

- who owns it
- what lock protects it
- when it changes

Locking rules must be explicit.

---

## 4. Do not hide cost

Public functions must make visible:

- locking
- allocation
- ownership transfer
- blocking
- failure conditions

Hidden cost is a design defect.

---

## 5. Keep architecture boundaries clean

Only architecture code may:

- define CPU execution state
- perform context switching
- control interrupts
- manage hardware state

Common code must never:

- assume register layout
- assume calling convention
- assume interrupt behavior

---

# Scheduler Bootstrap Rule

The scheduler starts in stages.

Idle initialization:

idle_init()

This creates and enqueues the idle thread.

The idle thread is not yet current.

Bootstrap boundary:

The initial current thread is established only by a context switch.

No code may assume a valid current thread before that boundary.

---

# Testing Rules

Tests must verify the surface implemented by the task.

Allowed tests:

- compile-time checks
- initialization behavior
- structural invariants
- minimal runtime behavior

Tests must not:

- depend on future functionality
- require hardware features not implemented
- assume concurrency not yet defined

---

# Change Control

Do not change existing behavior unless instructed.

Do not rename symbols without justification.

Do not refactor unrelated code.

Do not add fields or helpers without requirement.

---

# When Uncertain

Stop.

Read the task definition again.

If the behavior is not explicitly required:

Do not implement it.

Ask for clarification.

---

# Summary

One task.  
One surface.  

Build the kernel step by step.

Keep the repository simple.

Keep the behavior explicit.

Keep the system deterministic.
