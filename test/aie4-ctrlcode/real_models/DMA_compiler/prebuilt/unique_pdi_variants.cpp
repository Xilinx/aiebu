#include "prebuilt/unique_pdi_variants.hpp"

#include <fstream>
#include <mutex>
#include <stdexcept>

#include <nlohmann/json.hpp>

namespace me::prebuilt {

const OrderedPdiVariants& load_unique_pdi_variants(
    const std::filesystem::path& root_dir)
{
    static std::mutex mtx;
    static std::filesystem::path cached_root;
    static OrderedPdiVariants cached_variants;

    std::lock_guard<std::mutex> lock(mtx);

    if (cached_root == root_dir && !cached_variants.empty()) {
        return cached_variants;
    }

    const std::filesystem::path variants_path =
        root_dir / "prebuilt" / "unique_pdi_variants.json";
    std::ifstream ifs(variants_path);
    if (!ifs) {
        throw std::runtime_error(
            "Failed to open unique PDI variants JSON: " + variants_path.string());
    }

    nlohmann::ordered_json variants_json;
    ifs >> variants_json;

    OrderedPdiVariants variants;
    variants.reserve(variants_json.size());

    for (auto it = variants_json.begin(); it != variants_json.end(); ++it) {
        PdiVariant variant;
        const auto& entry = it.value();

        if (entry.contains("combined_kernel_names")
            && entry.at("combined_kernel_names").is_object()) {
            for (auto kn_it = entry.at("combined_kernel_names").begin();
                 kn_it != entry.at("combined_kernel_names").end();
                 ++kn_it) {
                variant.combined_kernel_names[kn_it.key()] =
                    kn_it.value().get<int>();
            }
        }

        if (entry.contains("combined_kernel_includes")
            && entry.at("combined_kernel_includes").is_array()) {
            variant.combined_kernel_includes =
                entry.at("combined_kernel_includes")
                    .get<std::vector<std::string>>();
        }

        variants.emplace_back(it.key(), std::move(variant));
    }

    cached_root = root_dir;
    cached_variants = std::move(variants);
    return cached_variants;
}

}  // namespace me::prebuilt
