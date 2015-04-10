/* absSym.h - macros to generate absolute symbols */

/*
 * Copyright (c) 2010, 2012-2014, Wind River Systems, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1) Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2) Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3) Neither the name of Wind River Systems nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef ABSSYM_H_
#define ABSSYM_H_

#ifdef __GNUC__

#define GEN_ABS_SYM_BEGIN(name) \
	extern void name(void); \
	void name(void)         \
	{

#define GEN_ABS_SYM_END }

#define GEN_ABS_SYM_HOST(name, value) \
	GEN_ABSOLUTE_SYM(CONFIG_##name, value)

#if defined(VXMICRO_ARCH_arm)

/*
 * GNU/ARM backend does not have a proper operand modifier which does not
 * produces prefix # followed by value, such as %0 for PowerPC, Intel, and
 * MIPS. The workaround performed here is using %B0 which converts
 * the value to ~(value). Thus "n"(~(value)) is set in operand constraint
 * to output (value) in the ARM specific GEN_OFFSET macro.
 */

#define GEN_ABSOLUTE_SYM(name, value)               \
	__asm__(".globl\t" #name "\n\t.equ\t" #name \
		",%B0"                              \
		"\n\t.type\t" #name ",%%object" :  : "n"(~(value)))

#elif defined(VXMICRO_ARCH_x86) || defined(VXMICRO_ARCH_arc)

#define GEN_ABSOLUTE_SYM(name, value)               \
	__asm__(".globl\t" #name "\n\t.equ\t" #name \
		",%c0"                              \
		"\n\t.type\t" #name ",@object" :  : "n"(value))

#else
#error processor architecture not supported
#endif

#elif defined(__DCC__)

#define GEN_ABS_SYM_BEGIN(name) GEN_ABSOLUTE_SYM(name, 0);

#define GEN_ABS_SYM_END

#define GEN_ABSOLUTE_SYM(name, value) \
	const long name __attribute__((absolute)) = (long)(value)

#define GEN_ABS_SYM_HOST(name, value) \
	const long CONFIG_##name __attribute__((absolute)) = (long)(value)

#else /* unsupported toolchain */

#define GEN_ABS_SYM_BEGIN(name) \
	#error GEN_ABS_SYM_BEGIN macro used with unsupported toolchain

#define GEN_ABS_SYM_END \
	#error GEN_ABS_SYM_END macro used with unsupported toolchain

#define GEN_ABSOLUTE_SYM(name) \
	#error GEN_ABSOLUTE_SYM macro used with unsupported toolchain

#endif /* end of "unsupported toolchain" */

#endif /* ABSSYM_H_ */
