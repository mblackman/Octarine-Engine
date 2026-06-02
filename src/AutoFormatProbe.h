#pragma once

// TEMPORARY probe to verify the autoformat CI workflow end-to-end. Never included anywhere, so it
// cannot affect the build. This PR is not meant to be merged — delete the file / close the PR after.
namespace octarine {

struct AutoFormatProbe {
  int value = 0;
  bool ready = false;
};

}  // namespace octarine
