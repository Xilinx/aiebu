#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace me::prebuilt {

struct PdiVariant {
    std::unordered_map<std::string, int> combined_kernel_names;
    std::vector<std::string> combined_kernel_includes;
};

using OrderedPdiVariants = std::vector<std::pair<std::string, PdiVariant>>;

// Loads prebuilt/unique_pdi_variants.json and preserves variant ordering.
const OrderedPdiVariants& load_unique_pdi_variants(
    const std::filesystem::path& root_dir);

}  // namespace me::prebuilt
