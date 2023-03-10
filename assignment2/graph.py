import matplotlib.pyplot as plt


withLocksOptimize = [[1, 5108], [2, 4353], [3, 3583], [4, 3439], [5, 3139], [6, 3563], [7, 2933], [8, 3391], [9, 3110], [10, 2495], [11, 3139], [12, 2865], [13, 2507], [14, 2331], [15, 2831], [16, 2847], [17, 2724], [18, 2768], [19, 2966], [20, 2450], [21, 2697], [22, 2471], [23, 2649], [24, 2583], [25, 2976], [26, 2816], [27, 3147], [28, 2610], [29, 2760], [30, 2578], [31, 3623], [32, 2897], [33, 2721], [34, 3009], [35, 2693], [36, 3072], [37, 2837], [38, 2463], [39, 2451], [40, 2489], [41, 3116], [42, 2780], [43, 2486], [44, 2688], [45, 2593], [46, 2577], [47, 2497], [48, 2594], [49, 2493], [50, 3555], [51, 2481], [52, 2504], [53, 2658], [54, 2510], [55, 2515], [56, 2500], [57, 2501], [58, 2506], [59, 2813], [60, 2526], [61, 2499], [62, 2494], [63, 2530], [64, 2515], [65, 2804], [66, 2512], [67, 2512], [68, 2518], [69, 2521], [70, 2513], [71, 2515], [72, 2891], [73, 2516], [74, 2516], [75, 2529], [76, 2515], [77, 2515], [78, 2517], [79, 2509], [80, 2520], [81, 2579], [82, 2521], [83, 2538], [84, 2524], [85, 2512], [86, 2536], [87, 2536], [88, 2512], [89, 2519], [90, 2516], [91, 2527], [92, 2536], [93, 2518], [94, 2530], [95, 2530], [96, 2598], [97, 2540], [98, 2546], [99, 2524], [100, 2533]]
withFIFOsOptimize = [[1, 4156], [2, 2956], [3, 3059], [4, 2837], [5, 2884], [6, 2230], [7, 1751], [8, 1691], [9, 1955], [10, 2411], [11, 2386], [12, 1849], [13, 2123], [14, 2137], [15, 2050], [16, 1794], [17, 1812], [18, 2245], [19, 2120], [20, 2004], [21, 1833], [22, 2328], [23, 2101], [24, 2537], [25, 2063], [26, 3243], [27, 2280], [28, 1918], [29, 2621], [30, 2270], [31, 1943], [32, 2138], [33, 1834], [34, 1838], [35, 2584], [36, 2171], [37, 2103], [38, 2295], [39, 1847], [40, 1862], [41, 2110], [42, 1873], [43, 2106], [44, 2007], [45, 1816], [46, 1861], [47, 1838], [48, 1854], [49, 1864], [50, 2132], [51, 1847], [52, 1999], [53, 1841], [54, 1866], [55, 2079], [56, 1925], [57, 1934], [58, 1865], [59, 1912], [60, 2110], [61, 1881], [62, 1881], [63, 1976], [64, 1862], [65, 1841], [66, 1865], [67, 1872], [68, 1862], [69, 1859], [70, 1878], [71, 1867], [72, 3154], [73, 1848], [74, 1868], [75, 1858], [76, 1868], [77, 1845], [78, 1874], [79, 1903], [80, 2150], [81, 1885], [82, 1872], [83, 1896], [84, 1906], [85, 1879], [86, 1885], [87, 1886], [88, 1892], [89, 1866], [90, 1881], [91, 1901], [92, 1890], [93, 1878], [94, 1897], [95, 1873], [96, 1882], [97, 1888], [98, 1881], [99, 1902], [100, 1868]]

serial = [[1, 3144], [1, 3209], [1, 3248], [1, 3152], [1, 3226], [1, 3137], [1, 3661], [1, 3202], [1, 3144], [1, 3148], [1, 3176], [1, 3243], [1, 3160], [1, 3210], [1, 3241], [1, 3290], [1, 3178], [1, 3173], [1, 3307], [1, 3181], [1, 3145], [1, 3174], [1, 3169], [1, 3161], [1, 3132], [1, 3163], [1, 3129], [1, 3143], [1, 3260], [1, 3167], [1, 3153], [1, 3148], [1, 3204], [1, 3136], [1, 3114], [1, 3318], [1, 3122], [1, 3135], [1, 3153], [1, 3120], [1, 3167], [1, 3607], [1, 3117], [1, 3270], [1, 3158], [1, 4404], [1, 3171], [1, 3157], [1, 3210], [1, 3890], [1, 3302], [1, 3132], [1, 3174], [1, 3356], [1, 3142], [1, 3184], [1, 3167], [1, 3194], [1, 3346], [1, 3104], [1, 3132], [1, 3175], [1, 3436], [1, 3411], [1, 3137], [1, 3190], [1, 3138], [1, 3161], [1, 3157], [1, 3138], [1, 3152], [1, 3179], [1, 3137], [1, 3208], [1, 3161], [1, 3115], [1, 3130], [1, 3152], [1, 3372], [1, 3136], [1, 3857], [1, 3141], [1, 3301], [1, 3125], [1, 3118], [1, 3316], [1, 3143], [1, 3175], [1, 3173], [1, 3276], [1, 3156], [1, 3251], [1, 3159], [1, 3239], [1, 3170], [1, 3297], [1, 3138], [1, 3172], [1, 3162], [1, 3193]]
withLocks = [[10, 2304], [10, 2276], [10, 2306], [10, 2281], [10, 2287], [10, 2292], [10, 2284], [10, 2295], [10, 2288], [10, 2319], [10, 2305], [10, 2322], [10, 2326], [10, 2309], [10, 2297], [10, 2304], [10, 2310], [10, 2301], [10, 2307], [10, 2465], [10, 2306], [10, 2316], [10, 2315], [10, 2340], [10, 2329], [10, 2298], [10, 2301], [10, 2312], [10, 2316], [10, 2313], [10, 2310], [10, 2307], [10, 2322], [10, 2313], [10, 2306], [10, 2315], [10, 2304], [10, 2312], [10, 2296], [10, 2318], [10, 2307], [10, 2308], [10, 2309], [10, 2303], [10, 2312], [10, 2291], [10, 2304], [10, 2312], [10, 2299], [10, 2328], [10, 2306], [10, 2318], [10, 2301], [10, 2296], [10, 2300], [10, 2319], [10, 2297], [10, 2304], [10, 2316], [10, 2307], [10, 2304], [10, 2302], [10, 2317], [10, 2305], [10, 2321], [10, 2306], [10, 2309], [10, 2305], [10, 2308], [10, 2332], [10, 2314], [10, 2311], [10, 2326], [10, 2300], [10, 2303], [10, 2311], [10, 2332], [10, 2306], [10, 2315], [10, 2303], [10, 2306], [10, 2304], [10, 2520], [10, 2305], [10, 2297], [10, 2324], [10, 2303], [10, 2338], [10, 2319], [10, 2311], [10, 2309], [10, 2316], [10, 2307], [10, 2298], [10, 2311], [10, 2307], [10, 2315], [10, 2296], [10, 2305], [10, 2340]]
withFIFOs = [[10, 1589], [10, 1666], [10, 1709], [10, 1672], [10, 1627], [10, 1665], [10, 1718], [10, 1717], [10, 1750], [10, 1704], [10, 1715], [10, 1727], [10, 1707], [10, 1716], [10, 1737], [10, 1739], [10, 1724], [10, 1761], [10, 1699], [10, 1724], [10, 1729], [10, 1732], [10, 1726], [10, 1725], [10, 1707], [10, 1736], [10, 1760], [10, 1749], [10, 1711], [10, 1763], [10, 1749], [10, 1736], [10, 1707], [10, 1744], [10, 1705], [10, 1763], [10, 1736], [10, 1748], [10, 1739], [10, 1719], [10, 1733], [10, 1690], [10, 1700], [10, 1734], [10, 1741], [10, 1680], [10, 1718], [10, 1742], [10, 2068], [10, 1717], [10, 1725], [10, 1728], [10, 1717], [10, 1737], [10, 1692], [10, 1752], [10, 1707], [10, 1738], [10, 1727], [10, 1739], [10, 1733], [10, 1761], [10, 1721], [10, 1691], [10, 1710], [10, 1725], [10, 1732], [10, 1716], [10, 1708], [10, 1721], [10, 1864], [10, 1742], [10, 1739], [10, 1729], [10, 1741], [10, 1824], [10, 1714], [10, 1684], [10, 1728], [10, 1735], [10, 1741], [10, 1744], [10, 1713], [10, 1702], [10, 1728], [10, 1782], [10, 1689], [10, 1730], [10, 1734], [10, 1708], [10, 1707], [10, 1742], [10, 1748], [10, 1959], [10, 1734], [10, 1695], [10, 1746], [10, 1741], [10, 1729], [10, 1717]]
allIimings = [[t[1] for t in serial], [t[1] for t in withLocks], [t[1] for t in withFIFOs]]

# line graph for optimization
plt.plot([x[0] for x in withLocksOptimize], [y[1] for y in withLocksOptimize])
plt.plot([x[0] for x in withFIFOsOptimize], [y[1] for y in withFIFOsOptimize])
plt.title("Processing Time vs. Number of Threads")
plt.xlabel("Number of Threads")
plt.ylabel("Time (ms)")
plt.show()

# boxplot of timings
fig, ax = plt.subplots()
ax.boxplot(allIimings)
ax.set_title('TImes for all 3 processes')
ax.set_xticklabels(['serial', 'withLocks', 'withFIFOs'])
ax.set_ylabel('Time (ms)')
plt.show()