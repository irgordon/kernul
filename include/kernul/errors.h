#ifndef KERNUL_ERRORS_H
#define KERNUL_ERRORS_H

/*
 * include/kernul/errors.h
 *
 * Kernel error codes. Phase 3.
 *
 * kern_err_t is defined in types.h. This header defines the named
 * values for that type and the rules for using them.
 *
 * Rules:
 *   - KERN_OK (zero) is the only success value.
 *   - All error values are negative.
 *   - Positive return values are subsystem-defined results (byte counts,
 *     offsets, etc.) and must be documented at each call site.
 *   - Every return value must be checked by the caller.
 *   - Ignoring an error is a defect that must be justified in a comment.
 *   - panic() is reserved for invariant failures, not for errors that
 *     could be returned to a caller.
 */

#include <kernul/types.h>

/* -------------------------------------------------------------------------
 * Success
 * ---------------------------------------------------------------------- */

#define KERN_OK           ( 0)

/* -------------------------------------------------------------------------
 * Error codes
 * ---------------------------------------------------------------------- */

#define KERN_ENOMEM       (-1)   /* allocation failed                      */
#define KERN_EINVAL       (-2)   /* invalid argument                       */
#define KERN_ENOENT       (-3)   /* entity does not exist                  */
#define KERN_EBUSY        (-4)   /* resource in use                        */
#define KERN_ENOSYS       (-5)   /* not implemented                        */
#define KERN_EFAULT       (-6)   /* bad address                            */
#define KERN_EPERM        (-7)   /* permission denied                      */
#define KERN_EIO          (-8)   /* I/O error                              */
#define KERN_EAGAIN       (-9)   /* try again                              */
#define KERN_EDEADLK      (-10)  /* deadlock detected                      */
#define KERN_EEXIST       (-11)  /* already exists                         */
#define KERN_ERANGE       (-12)  /* value out of range                     */
#define KERN_ETIMEDOUT    (-13)  /* operation timed out                    */

#endif /* KERNUL_ERRORS_H */
