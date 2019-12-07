from core import BetaScopeResult
from runMatch import runMatch, runlist_from_root
from plotMaker import PlotMaker

runs = {

    "HPK_32_4E14":{
            "reg":"(.*)(HPK)(.*)(3[pP]2|32)(.*)(4[eE]14)(.*)(S8664)(.*)",
            "out_fold":"HPK_32_4E14"
    },
    "HPK_32_8E14":{
            "reg":"(.*)(HPK)(.*)(3[pP]2|32)(.*)(8[eE]14)(.*)(S8664)(.*)",
            "out_fold":"HPK_32_8E14"
    },
    "HPK_32_1p5E15":{
            "reg":"(.*)(HPK)(.*)(3[pP]2|32)(.*)(1[pP]5[eE]15)(.*)(S8664)(.*)",
            "out_fold":"HPK_32_1p5E15"
    },
    "HPK_32_3E15":{
            "reg":"(.*)(HPK)(.*)(3[pP]2|32)(.*)(3[eE]15)(.*)(S8664)(.*)",
            "out_fold":"HPK_32_3E15"
    },
    "FBK_4E14":{
            "reg":"(.*)(FBK)(.*)(2[xX]2)(.*)(4[eE]14)(.*)(S8664)(.*)",
            "out_fold":"FBK_4E14"
    },
    "FBK_8E14":{
            "reg":"(.*)(FBK)(.*)(2[xX]2)(.*)(8[eE]14)(.*)(S8664)(.*)",
            "out_fold":"FBK_8E14"
    },
    "FBK_1p5E15":{
            "reg":"(.*)(FBK)(.*)(2[xX]2)(.*)(1[pP]5[eE]15)(.*)(S8664)(.*)",
            "out_fold":"FBK_1p5E15"
    },
    "FBK_3E15":{
            "reg":"(.*)(FBK)(.*)(2[xX]2)(.*)(3[eE]15)(.*)(S8664)(.*)",
            "out_fold":"FBK_3E15"
    },
    "FBK_HPK":{
            "reg":"(.*)(FBK|HPK)(.*)(S8664)(.*)",
            "out_fold":"FBK_HPK"
    },
}

runs = {
    "FBK_HPK":{
            "reg":"(.*)(FBK|HPK)(.*)(S8664)(.*)",
            "out_fold":"FBK_HPK"
    },
    #"FBK":{
    #        "reg":"(.*)(FBK)(.*)(UFSD3)(.*)(2[xX]2)(.*)(S8664)(.*)",
    #        "out_fold":"FBK"
    #},
    #"HPK32":{
    #        "reg":"(.*)(HPK)(.*)(3[pP]2|32)(.*)(S8664)(.*)",
    #        "out_fold":"HPK32"
    #},
    #"HPK_32_1p5E15":{
    #        "reg":"(.*)(HPK)(.*)(3[pP]2|32)(.*)(1[pP]5[eE]15)(.*)(S8664)(.*)",
    #        "out_fold":"HPK_32_1p5E15"
    #},
}


data_prep = [
{"nick_name":"NDL" ,"reg":"(.*)(NDL)(.*)(S8664)(.*)", "style":20, "color":2, "matched_runs":[] },
]

# (?!abc)
plot = PlotMaker("test_run.root")

fetched_data = plot.fetchRun(data_prep)
fetched_data.show_sensors()
fetched_data.makePlots("NDL")

if False:
    for run in runs:
        print runs[run]["reg"]
        data_prep = [
        {"nick_name":run, "reg":runs[run]["reg"], "style":20, "color":2, "matched_runs":[] },
        ]

        plot = PlotMaker("test_run.root")
        fetched_data = plot.fetchRun(data_prep)
        fetched_data.show_sensors()
        fetched_data.makePlots(runs[run]["out_fold"])

if False:
    data_prep = [
    {"nick_name":"HPK" ,"reg":"^((?!FBK).)*$", "style":20, "color":2, "matched_runs":[] },
    ]

    # (?!abc)
    plot = PlotMaker("test_run.root")

    fetched_data = plot.fetchRun(data_prep)

    #fetched_data.show_sensors()
    #raw_input()

    if False:
        ata_prep = [
        {"nick_name":"HPK" ,"reg":"^((?!2[xX]2).)*$", "style":21, "color":3, "matched_runs":[] },
        ]

        fetched_data = fetched_data.fetchRun(data_prep)
        fetched_data.show_sensors()
        raw_input()

    data_prep = [
    {"nick_name":"HPK" ,"reg":"^((?!CNM).)*$", "style":23, "color":2, "matched_runs":[] },
    ]

    fetched_data = fetched_data.fetchRun(data_prep)
    #fetched_data.show_sensors()
    #raw_input()


    data_prep = [
    {"nick_name":"HPK" ,"reg":"(.*)(HPK)(.*)(3[pP]2)(.*)(8[eE]14)(.*)(S8664)(.*)", "style":22, "color":2, "matched_runs":[] },
    ]

    fetched_data = fetched_data.fetchRun(data_prep)

    fetched_data.show_sensors()
    #raw_input()


    fetched_data.makePlots("my_exmaple")
