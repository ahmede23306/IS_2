#include "Visualizer.h"
#include "Utils.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cstdlib>

using namespace std;

static string escapeXml(const string& s) {

    string out;

    for (char c : s) {

        switch (c) {
        case '&': out += "&amp;";  break;
        case '<': out += "&lt;";   break;
        case '>': out += "&gt;";   break;
        default:  out += c;
        }
    }

    return out;
}

// Format a number with fixed decimals
static string fmt(double v, int decimals = 1) {

    ostringstream ss;

    ss << fixed << setprecision(decimals) << v;

    return ss.str();
}

// Visualize Support as an inline SVG bar chart inside an HTML file
void visualizeSupportBarChart(
    const FrequentLevels& allFrequentItemsets,
    double minSupport
) {

    // Colors per level (cycled if there are more levels)
    const char* colors[] = {
        "#4C78A8", "#F58518", "#54A24B",
        "#E45756", "#B279A2", "#72B7B2"
    };

    const int numColors = 6;

    // Flatten all itemsets with their level
    struct Bar {
        string name;
        double support;
        int level;
    };

    vector<Bar> bars;

    for (size_t k = 0; k < allFrequentItemsets.size(); ++k) {

        for (const auto& pair : allFrequentItemsets[k]) {

            bars.push_back({
                itemsetToString(pair.first),
                pair.second * 100.0,
                (int)k
                });
        }
    }

    if (bars.empty()) {
        return;
    }

    // Layout
    const int slot = 44;
    const int barW = 28;
    const int left = 70;
    const int right = 140;   // room for threshold label
    const int top = 95;      // room for title, legend, top value labels
    const int plotH = 320;
    // Room for rotated labels, based on the longest itemset name
    size_t maxLen = 0;
    for (const auto& b : bars) maxLen = max(maxLen, b.name.size());
    const int bottom = 30 + (int)(maxLen * 6);

    int plotW = (int)bars.size() * slot;
    int width = left + plotW + right;
    int height = top + plotH + bottom;

    ofstream html("support_chart.html");

    html << "<!DOCTYPE html><html><head><meta charset='utf-8'>"
        << "<title>Apriori Support Chart</title>"
        << "<style>body{font-family:Arial,sans-serif;margin:20px;}"
        << "svg{max-width:100%;height:auto;}</style></head><body>";

    html << "<svg xmlns='http://www.w3.org/2000/svg' "
        << "viewBox='0 0 " << width << " " << height << "' "
        << "width='" << width << "' height='" << height << "' "
        << "font-family='Arial,sans-serif'>";

    // Title
    html << "<text x='" << width / 2 << "' y='28' text-anchor='middle' "
        << "font-size='18' font-weight='bold'>"
        << "Frequent Itemsets Support (Min Support = "
        << fmt(minSupport * 100.0) << "%)</text>";

    // Gridlines and Y axis labels (0% to 100%, step 10)
    for (int p = 0; p <= 100; p += 10) {

        double y = top + plotH - (p / 100.0) * plotH;

        html << "<line x1='" << left << "' y1='" << y
            << "' x2='" << left + plotW << "' y2='" << y
            << "' stroke='#dddddd' stroke-width='1'/>";

        html << "<text x='" << left - 8 << "' y='" << y + 4
            << "' text-anchor='end' font-size='11' fill='#555'>"
            << p << "%</text>";
    }

    // Axes
    html << "<line x1='" << left << "' y1='" << top
        << "' x2='" << left << "' y2='" << top + plotH
        << "' stroke='#333' stroke-width='1.5'/>";

    html << "<line x1='" << left << "' y1='" << top + plotH
        << "' x2='" << left + plotW << "' y2='" << top + plotH
        << "' stroke='#333' stroke-width='1.5'/>";

    // Y axis title
    html << "<text transform='translate(18," << top + plotH / 2
        << ") rotate(-90)' text-anchor='middle' font-size='12' "
        << "fill='#333'>Support (%)</text>";

    // Bars, value labels, rotated X labels
    for (size_t i = 0; i < bars.size(); ++i) {

        const Bar& b = bars[i];

        double h = (b.support / 100.0) * plotH;
        double x = left + i * slot + (slot - barW) / 2.0;
        double y = top + plotH - h;
        double cx = x + barW / 2.0;

        html << "<rect x='" << x << "' y='" << y
            << "' width='" << barW << "' height='" << h
            << "' fill='" << colors[b.level % numColors]
            << "' rx='2'><title>" << escapeXml(b.name)
            << ": " << fmt(b.support, 2) << "%</title></rect>";

        html << "<text x='" << cx << "' y='" << y - 5
            << "' text-anchor='middle' font-size='11' fill='#222'>"
            << fmt(b.support) << "%</text>";

        html << "<text transform='translate(" << cx << ","
            << top + plotH + 14 << ") rotate(45)' "
            << "text-anchor='start' font-size='11' fill='#222'>"
            << escapeXml(b.name) << "</text>";
    }

    // Min support threshold (dashed line)
    double ty = top + plotH - (minSupport * plotH);

    html << "<line x1='" << left << "' y1='" << ty
        << "' x2='" << left + plotW << "' y2='" << ty
        << "' stroke='#D62728' stroke-width='2' "
        << "stroke-dasharray='8,5'/>";

    html << "<text x='" << left + plotW + 8 << "' y='" << ty + 4
        << "' text-anchor='start' font-size='11' fill='#D62728'>"
        << "Min Support " << fmt(minSupport * 100.0) << "%</text>";

    // Legend (one entry per level)
    int lx = left;

    for (size_t k = 0; k < allFrequentItemsets.size(); ++k) {

        html << "<rect x='" << lx << "' y='46' width='12' height='12' "
            << "fill='" << colors[k % numColors] << "'/>";

        html << "<text x='" << lx + 17 << "' y='56' font-size='12' "
            << "fill='#333'>L" << (k + 1) << "</text>";

        lx += 50;
    }

    html << "</svg></body></html>";

    html.close();

    cout << "\n[Chart saved successfully: support_chart.html]\n";

#ifdef _WIN32
    system("start support_chart.html");
#endif
}