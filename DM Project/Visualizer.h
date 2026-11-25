#pragma once

#include "Transaction.h"

// Generates support_chart.html containing an inline SVG bar chart
void visualizeSupportBarChart(
    const FrequentLevels& allFrequentItemsets,
    double minSupport
);