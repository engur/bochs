/////////////////////////////////////////////////////////////////////////
// $Id: avx512_conflict.cc 12437 2014-07-25 08:35:06Z vruppert $
/////////////////////////////////////////////////////////////////////////
//
//   Copyright (c) 2014 Stanislav Shwartsman
//          Written by Stanislav Shwartsman [sshwarts at sourceforge net]
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA B 02110-1301 USA
//
/////////////////////////////////////////////////////////////////////////

#define NEED_CPU_REG_SHORTCUTS 1
#include "bochs.h"
#include "cpu.h"
#define LOG_THIS BX_CPU_THIS_PTR

// AVX-512 conflict detection instructions

#if BX_SUPPORT_EVEX

#include "simd_int.h"
#include "scalar_arith.h"

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::VPCONFLICTD_MASK_VdqWdqR(bxInstruction_c *i)
{
  BxPackedAvxRegister op = BX_READ_AVX_REG(i->src());
  unsigned len = i->getVL();

  for (unsigned n=0; n < DWORD_ELEMENTS(len); n++) {
    op.vmm32u(n) = simd_pconflictd(&op, n);
  }

  if (i->opmask()) {
    avx512_write_regd_masked(i, &op, len, BX_READ_16BIT_OPMASK(i->opmask()));
  }
  else {
    BX_WRITE_AVX_REGZ(i->dst(), op, len);
  }

  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::VPCONFLICTQ_MASK_VdqWdqR(bxInstruction_c *i)
{
  BxPackedAvxRegister op = BX_READ_AVX_REG(i->src());
  unsigned len = i->getVL();

  for (unsigned n=0; n < QWORD_ELEMENTS(len); n++) {
    op.vmm64u(n) = simd_pconflictq(&op, n);
  }

  if (i->opmask()) {
    avx512_write_regq_masked(i, &op, len, BX_READ_8BIT_OPMASK(i->opmask()));
  }
  else {
    BX_WRITE_AVX_REGZ(i->dst(), op, len);
  }

  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::VPBROADCASTMB2Q_VdqKEbR(bxInstruction_c *i)
{
  simd_pbroadcastq(&BX_AVX_REG(i->dst()), (Bit64u) BX_READ_8BIT_OPMASK(i->src()), QWORD_ELEMENTS(i->getVL()));
  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::VPBROADCASTMW2D_VdqKEwR(bxInstruction_c *i)
{
  simd_pbroadcastd(&BX_AVX_REG(i->dst()), (Bit32u) BX_READ_16BIT_OPMASK(i->src()), DWORD_ELEMENTS(i->getVL()));
  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::VPLZCNTD_MASK_VdqWdqR(bxInstruction_c *i)
{
  BxPackedAvxRegister op = BX_READ_AVX_REG(i->src());
  unsigned len = i->getVL();

  for (unsigned n=0; n < DWORD_ELEMENTS(len); n++)
    op.vmm32u(n) = lzcntd(op.vmm32u(n));

  avx512_write_regd_masked(i, &op, len, BX_READ_16BIT_OPMASK(i->opmask()));
  BX_NEXT_INSTR(i);
}

BX_INSF_TYPE BX_CPP_AttrRegparmN(1) BX_CPU_C::VPLZCNTQ_MASK_VdqWdqR(bxInstruction_c *i)
{
  BxPackedAvxRegister op = BX_READ_AVX_REG(i->src());
  unsigned len = i->getVL();

  for (unsigned n=0; n < QWORD_ELEMENTS(len); n++)
    op.vmm64u(n) = lzcntq(op.vmm64u(n));

  avx512_write_regq_masked(i, &op, len, BX_READ_8BIT_OPMASK(i->opmask()));
  BX_NEXT_INSTR(i);
}

#endif