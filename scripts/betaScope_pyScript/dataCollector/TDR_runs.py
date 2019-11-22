marker_type = {
    "HPK":20,
    "FBK":21,
    "CNM":22,
    "NDL":23
}

color_type = {
    "HPK":1,
    "FBK":2,
    "CNM":3,
    "NDL":4
}

color_fluence = {
    "0":1,
    "1E14":2,
    "4E14":3,
    "8E14":4,
    "1E15":5,
    "1.5E15":6,
    "2.25E15":7,
    "3E15":8
}

marker_fluence = {
    "0":20,
    "1E14":21,
    "4E14":22,
    "8E14":23,
    "1E15":24,
    "1.5E15":25,
    "2.25E15":26,
    "3E15":27
}

HPK32 = [442, 606, 596, 542, 615, 477]
HPK32_metadata = {
        "general":
            {
                "title":"HPK Type 3.2",
                "temperature":"-30C"
            },
        "442":
            {
                "treename":"Sr_Run442",
                "fluence":"0",
                "nick_name":"W18 p1 LGE500"
            },
        "606":
            {
                "treename":"Sr_Run606",
                "fluence":"4E14",
                "nick_name":"W18 p5 LGE500"
            },
        "596":
            {
                "treename":"Sr_Run596",
                "fluence":"8E14",
                "nick_name":"W18 p4 LGE500"
            },
        "542":
            {
                "treename":"Sr_Run542",
                "fluence":"1.5E15",
                "nick_name":"W18 p5 LGE200"
            },
        "615":
            {
                "treename":"Sr_Run615",
                "fluence":"2.25E15",
                "nick_name":"W13 p3 LGE500"
            },
        "477":
            {
                "treename":"Sr_Run477",
                "fluence":"3E15",
                "nick_name":"W18 p4 LGE500"
            }
}

FBKW5 = [427, 435, 428, 692, 434]
FBKW5_metadata = {
        "general":
            {
                "title":"FBK UFSD3 W5 B+C",
                "temperature":"-30C"
            },
        "427":
            {
                "treename":"Sr_Run427",
                "fluence":"1E14",
                "nick_name":"2x2 #11"
            },
        "435":
            {
                "treename":"Sr_Run435",
                "fluence":"4E14",
                "nick_name":"2x2 #12"
            },
        "428":
            {
                "treename":"Sr_Run428",
                "fluence":"8E14",
                "nick_name":"2x2 #16"
            },
        "692":
            {
                "treename":"Sr_Run692",
                "fluence":"1.5E15",
                "nick_name":"2x2 #"
            },
        "434":
            {
                "treename":"Sr_Run434",
                "fluence":"3E15",
                "nick_name":"2x2 #17"
            },
}
