struct CITY
{
    int CityCode;
    CString CityName;
};

struct PROVINCE
{
    CString ProvinceCode;
    CString ProvinceName;
};

CITY CityArray[340] = 
{
{10,L"北京"},
{20,L"广州"},
{21,L"上海"},
{22,L"天津"},
{23,L"重庆"},
{24,L"沈阳"},
{25,L"南京"},
{27,L"武汉"},
{28,L"成都"},
{29,L"西安"},
{310,L"邯郸"},
{311,L"石家庄"},
{312,L"保定"},
{313,L"张家口"},
{314,L"承德"},
{315,L"唐山"},
{316,L"廊坊"},
{317,L"沧州"},
{318,L"衡水"},
{319,L"邢台"},
{335,L"秦皇岛"},
{349,L"朔州"},
{350,L"忻州"},
{351,L"太原"},
{352,L"大同"},
{353,L"阳泉"},
{354,L"榆次"},
{355,L"长治"},
{356,L"晋城"},
{357,L"临汾"},
{358,L"离石"},
{359,L"运城"},
{370,L"商丘"},
{371,L"郑州"},
{372,L"安阳"},
{373,L"新乡"},
{374,L"许昌"},
{375,L"平顶山"},
{376,L"信阳"},
{377,L"南阳"},
{378,L"开封"},
{379,L"洛阳"},
{391,L"焦作"},
{392,L"鹤壁"},
{393,L"濮阳"},
{394,L"周口"},
{395,L"漯河"},
{396,L"驻马店"},
{397,L"潢川"},
{398,L"三门峡"},
{410,L"铁岭"},
{411,L"大连"},
{412,L"鞍山"},
{413,L"抚顺"},
{414,L"本溪"},
{415,L"丹东"},
{416,L"锦州"},
{417,L"营口"},
{418,L"阜新"},
{419,L"辽阳"},
{421,L"朝阳"},
{427,L"盘锦"},
{429,L"葫芦岛"},
{431,L"长春"},
{432,L"吉林"},
{433,L"延吉"},
{434,L"四平"},
{435,L"通化"},
{436,L"白城"},
{437,L"辽源"},
{438,L"松原"},
{439,L"白山"},
{448,L"梅河口"},
{451,L"哈尔滨"},
{452,L"齐齐哈尔"},
{453,L"牡丹江"},
{454,L"佳木斯"},
{455,L"绥化"},
{456,L"黑河"},
{457,L"大兴安岭"},
{458,L"伊春"},
{459,L"大庆"},
{464,L"七台河"},
{467,L"鸡西"},
{468,L"鹤岗"},
{469,L"双鸭山"},
{470,L"海拉尔"},
{471,L"呼和浩特"},
{472,L"包头"},
{473,L"乌海"},
{474,L"集宁"},
{475,L"通辽"},
{476,L"赤峰"},
{477,L"东胜"},
{478,L"临河"},
{479,L"锡林浩特"},
{482,L"鸟兰浩特"},
{483,L"阿拉善左旗"},
{510,L"无锡"},
{511,L"镇江"},
{512,L"苏州"},
{513,L"南通"},
{514,L"扬州"},
{515,L"盐城"},
{516,L"徐州"},
{517,L"淮安"},
{518,L"连云港"},
{519,L"常州"},
{520,L"张家港"},
{523,L"泰州"},
{527,L"宿迁"},
{530,L"菏泽"},
{531,L"济南"},
{532,L"青岛"},
{533,L"淄博"},
{534,L"德州"},
{535,L"烟台"},
{536,L"潍坊"},
{537,L"济宁"},
{538,L"泰安"},
{539,L"临沂"},
{543,L"滨州"},
{546,L"东营"},
{550,L"滁州"},
{551,L"合肥"},
{552,L"蚌埠"},
{553,L"芜湖"},
{554,L"准南"},
{555,L"马鞍山"},
{556,L"安庆"},
{557,L"宿州"},
{558,L"阜阳"},
{559,L"黄山"},
{561,L"淮北"},
{562,L"铜陵"},
{563,L"宣城"},
{564,L"六安"},
{565,L"巢湖"},
{566,L"池州"},
{570,L"衢州"},
{571,L"杭州"},
{572,L"湖州"},
{573,L"嘉兴"},
{574,L"宁波"},
{575,L"绍兴"},
{576,L"台州"},
{577,L"温州"},
{578,L"丽水"},
{579,L"金华"},
{580,L"舟山"},
{591,L"福州"},
{592,L"厦门"},
{593,L"宁德"},
{594,L"莆田"},
{595,L"泉州"},
{596,L"漳州"},
{597,L"龙岩"},
{598,L"三明"},
{599,L"南平"},
{631,L"威海"},
{632,L"枣庄"},
{633,L"日照"},
{634,L"莱芜"},
{635,L"聊城"},
{660,L"汕尾"},
{661,L"潮阳"},
{662,L"阳江"},
{663,L"揭阳"},
{668,L"茂名"},
{691,L"西双版纳"},
{692,L"德宏"},
{701,L"鹰潭"},
{710,L"襄樊"},
{711,L"鄂州"},
{712,L"孝感"},
{713,L"黄冈"},
{714,L"黄石"},
{715,L"咸宁"},
{716,L"荆州"},
{717,L"宜昌"},
{718,L"恩施"},
{719,L"十堰"},
{722,L"随州"},
{724,L"荆门"},
{728,L"洪湖"},
{730,L"岳阳"},
{731,L"长沙"},
{732,L"湘潭"},
{733,L"株洲"},
{734,L"衡阳"},
{735,L"郴州"},
{736,L"常德"},
{737,L"益阳"},
{738,L"娄底"},
{739,L"邵阳"},
{743,L"吉首"},
{744,L"张家界"},
{745,L"怀化"},
{746,L"永州"},
{750,L"江门"},
{751,L"韶关"},
{752,L"惠州"},
{753,L"梅州"},
{754,L"汕头"},
{755,L"深圳"},
{756,L"珠海"},
{757,L"佛山"},
{758,L"肇庆"},
{759,L"湛江"},
{760,L"中山"},
{762,L"河源"},
{763,L"清远"},
{765,L"顺德"},
{766,L"云浮"},
{768,L"潮州"},
{769,L"东莞"},
{770,L"防城港"},
{771,L"南宁"},
{772,L"柳州"},
{773,L"桂林"},
{774,L"梧州"},
{775,L"玉林"},
{776,L"百色"},
{777,L"钦州"},
{778,L"河池"},
{779,L"北海"},
{790,L"新余"},
{791,L"南昌"},
{792,L"九江"},
{793,L"上饶"},
{794,L"抚州"},
{795,L"宜春"},
{796,L"吉安"},
{797,L"赣州"},
{798,L"景德镇"},
{799,L"萍乡"},
{812,L"攀枝花"},
{813,L"自贡"},
{816,L"绵阳"},
{817,L"南充"},
{818,L"达川"},
{825,L"遂宁"},
{826,L"广安"},
{827,L"巴中"},
{830,L"泸州"},
{831,L"宜宾"},
{832,L"内江"},
{833,L"乐山"},
{834,L"西昌"},
{835,L"雅安"},
{836,L"康定"},
{837,L"马尔康"},
{838,L"德阳"},
{839,L"广元"},
{851,L"贵阳"},
{852,L"遵义"},
{853,L"安顺"},
{854,L"都匀"},
{855,L"凯里"},
{856,L"铜仁"},
{857,L"毕节"},
{858,L"六盘水"},
{859,L"兴义"},
{870,L"昭通"},
{871,L"昆明"},
{872,L"大理"},
{873,L"个旧"},
{874,L"曲靖"},
{875,L"保山"},
{876,L"文山"},
{877,L"玉溪"},
{878,L"楚雄"},
{879,L"思茅"},
{881,L"东川"},
{883,L"临沧"},
{886,L"六库"},
{887,L"迪庆"},
{888,L"丽江"},
{890,L"儋州"},
{891,L"拉萨"},
{892,L"日喀则"},
{893,L"山南"},
{894,L"林芝"},
{895,L"昌都"},
{896,L"那曲"},
{897,L"阿里"},
{898,L"海口"},
{899,L"三亚"},
{901,L"塔城"},
{902,L"哈密"},
{903,L"和田"},
{906,L"阿勒泰"},
{908,L"阿图什"},
{909,L"博乐"},
{910,L"咸阳"},
{911,L"延安"},
{912,L"榆林"},
{913,L"渭南"},
{914,L"商州"},
{915,L"安康"},
{916,L"汉中"},
{917,L"宝鸡"},
{919,L"铜川"},
{930,L"临夏"},
{931,L"兰州"},
{932,L"定西"},
{933,L"平凉"},
{934,L"西峰"},
{935,L"武威"},
{936,L"张掖"},
{937,L"酒泉"},
{938,L"天水"},
{939,L"武都"},
{941,L"甘南州"},
{943,L"白银"},
{951,L"银川"},
{952,L"石嘴山"},
{953,L"吴忠"},
{954,L"固原"},
{970,L"海晏"},
{971,L"西宁"},
{972,L"海东"},
{973,L"同仁"},
{974,L"共和"},
{975,L"玛沁"},
{976,L"玉树"},
{977,L"德令哈"},
{978,L"门源"},
{979,L"格尔木"},
{988,L"喀会"},
{990,L"克拉玛依"},
{991,L"乌鲁木齐"},
{992,L"奎屯"},
{993,L"石河子"},
{994,L"昌吉"},
{995,L"吐鲁番"},
{996,L"库尔勒"},
{997,L"阿克苏"},
{998,L"喀什"},
{999,L"伊宁"}
};

PROVINCE ProvinceArray[34] = {
{L"bj",L"北京市"},
{L"sh",L"上海市"},
{L"tj",L"天津市"},
{L"cq",L"重庆市"},
{L"ah",L"安徽省"},
{L"fj",L"福建省"},
{L"gd",L"广东省"},
{L"gs",L"甘肃省"},
{L"gx",L"广西自治区"},
{L"gz",L"贵州省"},
{L"he",L"河北省"},
{L"hb",L"湖北省"},
{L"hl",L"黑龙江省"},
{L"hi",L"海南省"},
{L"ha",L"河南省"},
{L"hn",L"湖南省"},
{L"jl",L"吉林省"},
{L"js",L"江苏省"},
{L"jx",L"江西省"},
{L"ln",L"辽宁省"},
{L"nm",L"内蒙古自治区"},
{L"nx",L"宁夏自治区"},
{L"qh",L"青海省"},
{L"sc",L"四川省"},
{L"sd",L"山东省"},
{L"sx",L"山西省"},
{L"sn",L"陕西省"},
{L"xj",L"新疆自治区"},
{L"xz",L"西藏自治区"},
{L"yn",L"云南省"},
{L"zj",L"浙江省"},
{L"tw",L"台湾"},
{L"hk",L"香港特别行政区"},
{L"mo",L"澳门特别行政区"}
};