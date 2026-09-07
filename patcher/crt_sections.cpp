#pragma section(".CRT$XIA", read)
#pragma section(".CRT$XIZ", read)
#pragma section(".CRT$XCA", read)
#pragma section(".CRT$XCZ", read)
#pragma section(".CRT$XTA", read)
#pragma section(".CRT$XTZ", read)

extern "C"
{
	__declspec(allocate(".CRT$XIA")) void* __xi_a = nullptr;
	__declspec(allocate(".CRT$XIZ")) void* __xi_z = nullptr;
	__declspec(allocate(".CRT$XCA")) void* __xc_a = nullptr;
	__declspec(allocate(".CRT$XCZ")) void* __xc_z = nullptr;
	__declspec(allocate(".CRT$XTA")) void* __xt_a = nullptr;
	__declspec(allocate(".CRT$XTZ")) void* __xt_z = nullptr;
}

#include <stdlib.h>

// This plugin builds without C++ exceptions (no try/catch/throw anywhere in the
// codebase, no /EHsc). Plain `new` is normally guaranteed by the standard to never
// return null on failure -- callers throughout this codebase rely on that and don't
// null-check the result. Since we can't throw std::bad_alloc here without exception
// support, we fail loudly and deterministically via abort() instead of silently
// handing back null and letting some unrelated line crash on a null-pointer write
// later, at a point that's much harder to diagnose than an allocation failure.
static void* AllocOrAbort(size_t size)
{
	void* ptr = malloc(size);
	if (!ptr && size != 0)
	{
		abort();
	}
	return ptr;
}

void* operator new(size_t size)
{
	return AllocOrAbort(size);
}

void operator delete(void* ptr) noexcept
{
	free(ptr);
}

void* operator new[](size_t size)
{
	return AllocOrAbort(size);
}

void operator delete[](void* ptr) noexcept
{
	free(ptr);
}

void operator delete(void* ptr, size_t size) noexcept
{
	(void)size;
	free(ptr);
}

void operator delete[](void* ptr, size_t size) noexcept
{
	(void)size;
	free(ptr);
}
