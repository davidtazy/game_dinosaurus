#pragma once

#include "Nothing.h"
#include "OneOf.h"

template <typename Action>
struct Maybe : public OneOf<Action, Nothing> {
  using OneOf<Action, Nothing>::OneOf;
};
