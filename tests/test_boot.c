/*
 * tests/test_boot.c
 *
 * Deterministic tests for boot_info_validate(). Phase 0.
 *
 * Tests run in a hosted environment. No hardware required.
 * Each test function returns 0 on pass, 1 on failure.
 */

#include <kernul/boot.h>
#include <kernul/errors.h>
#include <kernul/mm.h>
#include <kernul/types.h>

/* -------------------------------------------------------------------------
 * Minimal test harness
 * ---------------------------------------------------------------------- */

static int tests_run    = 0;
static int tests_failed = 0;

#define CHECK(label, expr) \
    do { \
        tests_run++; \
        if (!(expr)) { \
            tests_failed++; \
        } \
    } while (0)

/* -------------------------------------------------------------------------
 * boot_info_validate tests
 * ---------------------------------------------------------------------- */

static void test_valid_minimal(void)
{
    boot_info_t info = {0};

    info.regions[0].base = 0x100000;
    info.regions[0].end  = 0x200000;
    info.regions[0].type = PHYS_REGION_KERNEL;
    info.regions[1].base = 0x200000;
    info.regions[1].end  = 0x1000000;
    info.regions[1].type = PHYS_REGION_USABLE;
    info.num_regions       = 2;
    info.kernel_phys_start = 0x100000;
    info.kernel_phys_end   = 0x200000;

    CHECK("valid minimal boot_info passes",
          boot_info_validate(&info) == KERN_OK);
}

static void test_zero_regions(void)
{
    boot_info_t info = {0};
    info.num_regions = 0;

    CHECK("zero regions fails validation",
          boot_info_validate(&info) == KERN_EINVAL);
}

static void test_too_many_regions(void)
{
    boot_info_t info = {0};
    info.num_regions = BOOT_MAX_PHYS_REGIONS + 1;

    CHECK("num_regions exceeding max fails validation",
          boot_info_validate(&info) == KERN_EINVAL);
}

static void test_unaligned_base(void)
{
    boot_info_t info = {0};

    /* base is not page-aligned */
    info.regions[0].base = 0x100001;
    info.regions[0].end  = 0x200000;
    info.regions[0].type = PHYS_REGION_USABLE;
    info.num_regions     = 1;

    CHECK("unaligned region base fails validation",
          boot_info_validate(&info) == KERN_EINVAL);
}

static void test_unaligned_end(void)
{
    boot_info_t info = {0};

    info.regions[0].base = 0x100000;
    info.regions[0].end  = 0x200001;   /* not page-aligned */
    info.regions[0].type = PHYS_REGION_USABLE;
    info.num_regions     = 1;

    CHECK("unaligned region end fails validation",
          boot_info_validate(&info) == KERN_EINVAL);
}

static void test_inverted_range(void)
{
    boot_info_t info = {0};

    info.regions[0].base = 0x200000;
    info.regions[0].end  = 0x100000;   /* end < base */
    info.regions[0].type = PHYS_REGION_USABLE;
    info.num_regions     = 1;

    CHECK("inverted range (end < base) fails validation",
          boot_info_validate(&info) == KERN_EINVAL);
}

static void test_kernel_extent_no_kernel_region(void)
{
    boot_info_t info = {0};

    /* Kernel extent set but no matching PHYS_REGION_KERNEL entry */
    info.regions[0].base   = 0x100000;
    info.regions[0].end    = 0x1000000;
    info.regions[0].type   = PHYS_REGION_USABLE;
    info.num_regions         = 1;
    info.kernel_phys_start   = 0x100000;
    info.kernel_phys_end     = 0x200000;

    CHECK("kernel extent without KERNEL region entry fails validation",
          boot_info_validate(&info) == KERN_EINVAL);
}

/* -------------------------------------------------------------------------
 * tests/test_mm.c — PMM tests. Phase 2.
 * ---------------------------------------------------------------------- */

static void test_phys_alloc_basic(void)
{
    /*
     * Allocate a page and verify the address is non-zero and page-aligned.
     *
     * Note: mm_phys_init() must have been called before this test runs.
     * The test runner below calls it with a synthetic range.
     */
    phys_addr_t p = mm_phys_alloc();
    CHECK("mm_phys_alloc returns non-zero", p != 0);
    CHECK("mm_phys_alloc returns page-aligned address",
          mm_is_page_aligned(p));
}

static void test_phys_alloc_count_decreases(void)
{
    u64 before = mm_phys_free_count();
    phys_addr_t p = mm_phys_alloc();
    u64 after  = mm_phys_free_count();

    CHECK("allocation decreases free count by one",
          p != 0 && after == before - 1);

    mm_phys_free(p);
}

static void test_phys_alloc_sequential(void)
{
    phys_addr_t a = mm_phys_alloc();
    phys_addr_t b = mm_phys_alloc();

    CHECK("two allocations return different addresses",
          a != 0 && b != 0 && a != b);

    mm_phys_free(a);
    mm_phys_free(b);
}

/* -------------------------------------------------------------------------
 * assert.h tests. Phase 3.
 *
 * The passing path of ASSERT() is tested: if the process reaches the
 * check after the macro, the macro did not halt on a true condition.
 *
 * The failing path (assert_fail itself) is not testable in this harness
 * without a crash-recovery mechanism. It is verified by code review of
 * core/assert.c.
 * ---------------------------------------------------------------------- */

#include <kernul/assert.h>

static void test_assert_true_does_not_halt(void)
{
    ASSERT(1 == 1);
    CHECK("ASSERT on true condition does not halt", 1);
}

static void test_assert_nonzero_does_not_halt(void)
{
    int x = 42;
    ASSERT(x > 0);
    CHECK("ASSERT on non-zero expression does not halt", 1);
}

/*
 * Synthetic physical memory layout for PMM tests.
 * These values are chosen to avoid conflicting with the test binary itself
 * in a hosted environment. The PMM stub does not actually map anything,
 * so any page-aligned values work for structure testing.
 */
#define TEST_MEM_START       0x01000000UL
#define TEST_MEM_END         0x02000000UL
#define TEST_KERNEL_START    0x01000000UL
#define TEST_KERNEL_END      0x01100000UL

int main(void)
{
    /* assert tests */
    test_assert_true_does_not_halt();
    test_assert_nonzero_does_not_halt();

    /* boot contract tests */
    test_valid_minimal();
    test_zero_regions();
    test_too_many_regions();
    test_unaligned_base();
    test_unaligned_end();
    test_inverted_range();
    test_kernel_extent_no_kernel_region();

    /* PMM tests — initialize with synthetic range first */
    mm_phys_init(TEST_MEM_START,    TEST_MEM_END,
                 TEST_KERNEL_START, TEST_KERNEL_END);

    test_phys_alloc_basic();
    test_phys_alloc_count_decreases();
    test_phys_alloc_sequential();

    if (tests_failed == 0) {
        return 0;
    } else {
        return 1;
    }
}
