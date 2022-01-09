// Copyright (c) 2004-2022 Tomáš Oberhuber et al.
//
// This file is part of TNL - Template Numerical Library (https://tnl-project.org/)
//
// SPDX-License-Identifier: MIT

// Implemented by: Tomas Oberhuber, Jakub Klinkovsky

#pragma once

namespace TNL {
namespace Algorithms {
namespace detail {

enum class ScanType
{
   Exclusive,
   Inclusive
};

enum class ScanPhaseType
{
   WriteInFirstPhase,
   WriteInSecondPhase
};

}  // namespace detail
}  // namespace Algorithms
}  // namespace TNL
