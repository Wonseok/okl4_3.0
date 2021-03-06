#ifndef _L4_PAGE_H
#define _L4_PAGE_H

#include <asm/macros.h>

/* Get our architecture page info */
#include INC_SYSTEM2(page.h)

#ifdef CONFIG_FLATMEM
#ifndef __ASSEMBLY__
extern unsigned long            pfn_offset;
#define pfn_valid(pfn)		((pfn) < max_mapnr)
#define ARCH_PFN_OFFSET         pfn_offset
#endif
#endif /* CONFIG_DISCONTIGMEM */

#include <asm-generic/memory_model.h>

#define PAGE_SIZE	(1UL << PAGE_SHIFT)
#define PAGE_MASK	(~(PAGE_SIZE-1))

#ifdef __KERNEL__

#ifndef __ASSEMBLY__

typedef struct {
	unsigned long page_base;    /* ADDRESS >> PAGE_SHIFT */
	unsigned long pages;	    /* Number of pages in this area */
} bootmem_area_t;

#define STRICT_MM_TYPECHECKS

struct page;

#define clear_page(addr)	memset((void*)(addr), 0, PAGE_SIZE)
#define copy_page(to, from)	memcpy((void *)(to), (void *)(from), PAGE_SIZE)

extern void clear_user_page(void *addr, unsigned long vaddr,
	struct page *page);
extern void copy_user_page(void *vto, void *vfrom, unsigned long vaddr,
	struct page *to);

#ifdef STRICT_MM_TYPECHECKS
/*
 * These are used to make use of C type-checking..
 */
typedef struct { unsigned long pte; } pte_t;
typedef struct { unsigned long pmd; } pmd_t;
typedef struct { unsigned long pgd; } pgd_t;
typedef struct { unsigned long pgprot; } pgprot_t;

#define pte_val(x)	((x).pte)
#define pmd_val(x)	((x).pmd)
#define pgd_val(x)	((x).pgd)
#define pgprot_val(x)	((x).pgprot)

#define __pte(x)	((pte_t) { (x) } )
#define __pmd(x)	((pmd_t) { (x) } )
#define __pgd(x)	((pgd_t) { (x) } )
#define __pgprot(x)	((pgprot_t) { (x) } )

#else
/*
 * .. while these make it easier on the compiler
 */
typedef unsigned long pte_t;
typedef unsigned long pmd_t;
typedef unsigned long pgd_t;
typedef unsigned long pgprot_t;

#define pte_val(x)	(x)
#define pmd_val(x)	(x)
#define pgd_val(x)	(x)
#define pgprot_val(x)	(x)

#define __pte(x)	(x)
#define __pgd(x)	(x)
#define __pgprot(x)	(x)

#endif /* STRICT_MM_TYPECHECKS */

/* Pure 2^n version of get_order */
extern __inline__ int get_order(unsigned long size)
{
	int order;

	size = (size-1) >> (PAGE_SHIFT-1);
	order = -1;
	do {
		size >>= 1;
		order++;
	} while (size);
	return order;
}

#endif /* !__ASSEMBLY__ */

/* to align the pointer to the (next) page boundary */
#define PAGE_ALIGN(addr)	(((addr)+PAGE_SIZE-1)&PAGE_MASK)
#define PAGE_OFFSET		0

#define __pa(x)			((unsigned long) (x) - PAGE_OFFSET)
#define __va(x)			((void *)((unsigned long) (x) + PAGE_OFFSET))

#define virt_addr_valid(kaddr)	pfn_valid(__pa(kaddr) >> PAGE_SHIFT)

#define page_to_phys(page)	(page_to_pfn(page) << PAGE_SHIFT)
#define virt_to_page(kaddr)	pfn_to_page(__pa(kaddr) >> PAGE_SHIFT)

#define VM_DATA_DEFAULT_FLAGS	(VM_READ | VM_WRITE | VM_EXEC |		\
				 VM_MAYREAD | VM_MAYWRITE | VM_MAYEXEC)

#endif /* __KERNEL__ */

#endif /* _L4_PAGE_H */
