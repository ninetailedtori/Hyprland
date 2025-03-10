#include "LayoutManager.hpp"

CLayoutManager::CLayoutManager() {
    m_vLayouts.emplace_back(std::make_pair<>("dwindle", &m_cDwindleLayout));
    m_vLayouts.emplace_back(std::make_pair<>("master", &m_cMasterLayout));
}

CIHyprLayout* CLayoutManager::getCurrentLayout() {
    return m_vLayouts[m_iCurrentLayoutID].second;
}

void CLayoutManager::switchToLayout(std::string layout) {
    for (size_t i = 0; i < m_vLayouts.size(); ++i) {
        if (m_vLayouts[i].first == layout) {
            if (i == (size_t)m_iCurrentLayoutID)
                return;

            getCurrentLayout()->onDisable();
            m_iCurrentLayoutID = i;
            getCurrentLayout()->onEnable();
            return;
        }
    }

    NDebug::log(ERR, "Unknown layout!");
}

bool CLayoutManager::addLayout(const std::string& name, IyprLayout* layout) {
    if (std::find_if(m_vLayouts.begin(), m_vLayouts.end(), [&](const auto& other) { return other.first == name || other.second == layout; }) != m_vLayouts.end())
        return false;

    m_vLayouts.emplace_back(std::make_pair<>(name, layout));

    NDebug::log(LOG, "Added new layout {} at {:x}", name, (uintptr_t)layout);

    return true;
}

bool CLayoutManager::removeLayout(CIHyprLayout* layout) {
    const auto IT = std::find_if(m_vLayouts.begin(), m_vLayouts.end(), [&](const auto& other) { return other.second == layout; });

    if (IT == m_vLayouts.end() || IT->first == "dwindle" || IT->first == "master")
        return false;

    if (m_iCurrentLayoutID == IT - m_vLayouts.begin())
        switchToLayout("dwindle");

    NDebug::log(LOG, "Removed a layout {} at {:x}", IT->first, (uintptr_t)layout);

    std::erase(m_vLayouts, *IT);

    return true;
}

std::vector<std::string> CLayoutManager::getAllLayoutNames() {
    std::vector<std::string> results(m_vLayouts.size());
    for (size_t i = 0; i < m_vLayouts.size(); ++i)
        results[i] = m_vLayouts[i].first;
    return results;
}
