/*
 * Copyright (c) 2016, Graphics Lab, Georgia Tech Research Corporation
 * Copyright (c) 2016, Humanoid Lab, Georgia Tech Research Corporation
 * Copyright (c) 2016, Personal Robotics Lab, Carnegie Mellon University
 * All rights reserved.
 *
 * This file is provided under the following "BSD-style" License:
 *   Redistribution and use in source and binary forms, with or
 *   without modification, are permitted provided that the following
 *   conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 *   USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *   POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef DART_DYNAMICS_SKELETONVIRIQNDRNEA_HPP_
#define DART_DYNAMICS_SKELETONVIRIQNDRNEA_HPP_

#include <Eigen/Dense>

#include "dart/common/AspectWithVersion.hpp"
#include "dart/dynamics/Skeleton.hpp"

namespace dart {
namespace dynamics {

// class Skeleton;

namespace detail {

struct SkeletonViRiqnDrneaState
{
  /// Constructor
  SkeletonViRiqnDrneaState();

  /// Destructor
  virtual ~SkeletonViRiqnDrneaState() = default;
};

} // namespace detail

//==============================================================================
class SkeletonViRiqnDrnea final
    : public common::AspectWithState<SkeletonViRiqnDrnea,
                                     detail::SkeletonViRiqnDrneaState,
                                     Skeleton>
{
public:
  using Base = common::AspectWithState<SkeletonViRiqnDrnea,
                                       detail::SkeletonViRiqnDrneaState,
                                       Skeleton>;

  using GradientMatrix = Eigen::Matrix<double, 6, Eigen::Dynamic>;

  enum TerminalCondition
  {
    Invalid,
    StaticSkeleton,
    MaximumIteration,
    Tolerance
  };

  SkeletonViRiqnDrnea(const StateData& state = StateData());

  SkeletonViRiqnDrnea(const SkeletonViRiqnDrnea&) = delete;

  void initialize();

  void setTolerance(double tol);

  double getTolerance() const;

  void setMaxIternation(std::size_t iter);

  std::size_t getMaxIteration() const;

  TerminalCondition integrate();

protected:
  void setComposite(common::Composite* newComposite) override;

  void loseComposite(common::Composite* oldComposite) override;

  void setPrevPositions(const Eigen::VectorXd& prevPositions);

  Eigen::VectorXd getPrevPositions() const;

  void setNextPositions(const Eigen::VectorXd& nextPositions);

  /// Evaluate forced discrete Euler-Lagrange (DEL) equation given next
  /// configurations.
  Eigen::VectorXd evaluateDel(const Eigen::VectorXd& nextPositions);

public:
  Eigen::MatrixXd evaluateDelDeriv(const Eigen::VectorXd& nextPositions);

  Eigen::VectorXd getError() const;

  void stepForward(const Eigen::VectorXd& nextPositions);

protected:
  double mTolerance{1e-9};

  std::size_t mMaxIteration{30u};
};

} // namespace dynamics
} // namespace dart

#endif // DART_DYNAMICS_SKELETONVIRIQNDRNEA_HPP_
