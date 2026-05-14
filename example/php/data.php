<?php

function CreateParams() {
    global $ydb;

    $seriesData = [
        [1, "IT Crowd", $ydb->YdbInstantParseIso8601("2006-02-03", NULL), <<<END
            The IT Crowd is a British sitcom produced by Channel 4, written by 
            Graham Linehan, produced by 
            Ash Atalla and starring Chris O'Dowd, Richard Ayoade, Katherine 
            Parkinson, and Matt Berry.
         END],
        [2, "Silicon Valley", $ydb->YdbInstantParseIso8601("2014-04-06", NULL), <<<END
            Silicon Valley is an American comedy television series created by 
            Mike Judge, John Altschuler and 
            Dave Krinsky. The series focuses on five young men who founded a 
            startup company in Silicon Valley.
         END]];

    $seasonsData = [
        [1, 1, "Season 1", $ydb->YdbInstantParseIso8601("2006-02-03", NULL),
         $ydb->YdbInstantParseIso8601("2006-03-03", NULL)],
        [1, 2, "Season 2", $ydb->YdbInstantParseIso8601("2007-08-24", NULL),
         $ydb->YdbInstantParseIso8601("2007-09-28", NULL)],
        [1, 3, "Season 3", $ydb->YdbInstantParseIso8601("2008-11-21", NULL),
         $ydb->YdbInstantParseIso8601("2008-12-26", NULL)],
        [1, 4, "Season 4", $ydb->YdbInstantParseIso8601("2010-06-25", NULL),
         $ydb->YdbInstantParseIso8601("2010-07-30", NULL)],
        [2, 1, "Season 1", $ydb->YdbInstantParseIso8601("2014-04-06", NULL),
         $ydb->YdbInstantParseIso8601("2014-06-01", NULL)],
        [2, 2, "Season 2", $ydb->YdbInstantParseIso8601("2015-04-12", NULL),
         $ydb->YdbInstantParseIso8601("2015-06-14", NULL)],
        [2, 3, "Season 3", $ydb->YdbInstantParseIso8601("2016-04-24", NULL),
         $ydb->YdbInstantParseIso8601("2016-06-26", NULL)],
        [2, 4, "Season 4", $ydb->YdbInstantParseIso8601("2017-04-23", NULL),
         $ydb->YdbInstantParseIso8601("2017-06-25", NULL)],
        [2, 5, "Season 5", $ydb->YdbInstantParseIso8601("2018-03-25", NULL),
         $ydb->YdbInstantParseIso8601("2018-05-13", NULL)]];

    $episodesData = [
        [1, 1, 1, "Yesterday's Jam",
         $ydb->YdbInstantParseIso8601("2006-02-03", NULL)],
        [1, 1, 2, "Calamity Jen", $ydb->YdbInstantParseIso8601("2006-02-03", NULL)],
        [1, 1, 3, "Fifty-Fifty", $ydb->YdbInstantParseIso8601("2006-02-10", NULL)],
        [1, 1, 4, "The Red Door", $ydb->YdbInstantParseIso8601("2006-02-17", NULL)],
        [1, 1, 5, "The Haunting of Bill Crouse",
         $ydb->YdbInstantParseIso8601("2006-02-24", NULL)],
        [1, 1, 6, "Aunt Irma Visits",
         $ydb->YdbInstantParseIso8601("2006-03-03", NULL)],
        [1, 2, 1, "The Work Outing",
         $ydb->YdbInstantParseIso8601("2006-08-24", NULL)],
        [1, 2, 2, "Return of the Golden Child",
         $ydb->YdbInstantParseIso8601("2007-08-31", NULL)],
        [1, 2, 3, "Moss and the German",
         $ydb->YdbInstantParseIso8601("2007-09-07", NULL)],
        [1, 2, 4, "The Dinner Party",
         $ydb->YdbInstantParseIso8601("2007-09-14", NULL)],
        [1, 2, 5, "Smoke and Mirrors",
         $ydb->YdbInstantParseIso8601("2007-09-21", NULL)],
        [1, 2, 6, "Men Without Women",
         $ydb->YdbInstantParseIso8601("2007-09-28", NULL)],
        [1, 3, 1, "From Hell", $ydb->YdbInstantParseIso8601("2008-11-21", NULL)],
        [1, 3, 2, "Are We Not Men?",
         $ydb->YdbInstantParseIso8601("2008-11-28", NULL)],
        [1, 3, 3, "Tramps Like Us", $ydb->YdbInstantParseIso8601("2008-12-05", NULL)],
        [1, 3, 4, "The Speech", $ydb->YdbInstantParseIso8601("2008-12-12", NULL)],
        [1, 3, 5, "Friendface", $ydb->YdbInstantParseIso8601("2008-12-19", NULL)],
        [1, 3, 6, "Cacountdar Geeks", $ydb->YdbInstantParseIso8601("2008-12-26", NULL)],
        [1, 4, 1, "Jen The Fredo", $ydb->YdbInstantParseIso8601("2010-06-25", NULL)],
        [1, 4, 2, "The Final Countdown",
         $ydb->YdbInstantParseIso8601("2010-07-02", NULL)],
        [1, 4, 3, "Something Happened",
         $ydb->YdbInstantParseIso8601("2010-07-09", NULL)],
        [1, 4, 4, "Italian For Beginners",
         $ydb->YdbInstantParseIso8601("2010-07-16", NULL)],
        [1, 4, 5, "Bad Boys", $ydb->YdbInstantParseIso8601("2010-07-23", NULL)],
        [1, 4, 6, "Reynholm vs Reynholm",
         $ydb->YdbInstantParseIso8601("2010-07-30", NULL)],
        [2, 1, 1, "Minimum Viable Product",
         $ydb->YdbInstantParseIso8601("2014-04-06", NULL)],
        [2, 1, 2, "The Cap Table", $ydb->YdbInstantParseIso8601("2014-04-13", NULL)],
        [2, 1, 3, "Articles of Incorporation",
         $ydb->YdbInstantParseIso8601("2014-04-20", NULL)],
        [2, 1, 4, "Fiduciary Duties",
         $ydb->YdbInstantParseIso8601("2014-04-27", NULL)],
        [2, 1, 5, "Signaling Risk", $ydb->YdbInstantParseIso8601("2014-05-04", NULL)],
        [2, 1, 6, "Third Party Insourcing",
         $ydb->YdbInstantParseIso8601("2014-05-11", NULL)],
        [2, 1, 7, "Proof of Concept",
         $ydb->YdbInstantParseIso8601("2014-05-18", NULL)],
        [2, 1, 8, "Optimal Tip-to-Tip Efficiency",
         $ydb->YdbInstantParseIso8601("2014-06-01", NULL)],
        [2, 2, 1, "Sand Hill Shuffle",
         $ydb->YdbInstantParseIso8601("2015-04-12", NULL)],
        [2, 2, 2, "Runaway Devaluation",
         $ydb->YdbInstantParseIso8601("2015-04-19", NULL)],
        [2, 2, 3, "Bad Money", $ydb->YdbInstantParseIso8601("2015-04-26", NULL)],
        [2, 2, 4, "The Lady", $ydb->YdbInstantParseIso8601("2015-05-03", NULL)],
        [2, 2, 5, "Server Space", $ydb->YdbInstantParseIso8601("2015-05-10", NULL)],
        [2, 2, 6, "Homicide", $ydb->YdbInstantParseIso8601("2015-05-17", NULL)],
        [2, 2, 7, "Adult Content", $ydb->YdbInstantParseIso8601("2015-05-24", NULL)],
        [2, 2, 8, "White Hat/Black Hat",
         $ydb->YdbInstantParseIso8601("2015-05-31", NULL)],
        [2, 2, 9, "Binding Arbitration",
         $ydb->YdbInstantParseIso8601("2015-06-07", NULL)],
        [2, 2, 10, "Two Days of the Condor",
         $ydb->YdbInstantParseIso8601("2015-06-14", NULL)],
        [2, 3, 1, "Founder Friendly",
         $ydb->YdbInstantParseIso8601("2016-04-24", NULL)],
        [2, 3, 2, "Two in the Box", $ydb->YdbInstantParseIso8601("2016-05-01", NULL)],
        [2, 3, 3, "Meinertzhagen's Haversack",
         $ydb->YdbInstantParseIso8601("2016-05-08", NULL)],
        [2, 3, 4, "Maleant Data Systems Solutions",
         $ydb->YdbInstantParseIso8601("2016-05-15", NULL)],
        [2, 3, 5, "The Empty Chair",
         $ydb->YdbInstantParseIso8601("2016-05-22", NULL)],
        [2, 3, 6, "Bachmanity Insanity",
         $ydb->YdbInstantParseIso8601("2016-05-29", NULL)],
        [2, 3, 7, "To Build a Better Beta",
         $ydb->YdbInstantParseIso8601("2016-06-05", NULL)],
        [2, 3, 8, "Bachman's Earnings Over-Ride",
         $ydb->YdbInstantParseIso8601("2016-06-12", NULL)],
        [2, 3, 9, "Daily Active Users",
         $ydb->YdbInstantParseIso8601("2016-06-19", NULL)],
        [2, 3, 10, "The Uptick", $ydb->YdbInstantParseIso8601("2016-06-26", NULL)],
        [2, 4, 1, "Success Failure",
         $ydb->YdbInstantParseIso8601("2017-04-23", NULL)],
        [2, 4, 2, "Terms of Service",
         $ydb->YdbInstantParseIso8601("2017-04-30", NULL)],
        [2, 4, 3, "Intellectual Property",
         $ydb->YdbInstantParseIso8601("2017-05-07", NULL)],
        [2, 4, 4, "Teambuilding Exercise",
         $ydb->YdbInstantParseIso8601("2017-05-14", NULL)],
        [2, 4, 5, "The Blood Boy", $ydb->YdbInstantParseIso8601("2017-05-21", NULL)],
        [2, 4, 6, "Customer Service",
         $ydb->YdbInstantParseIso8601("2017-05-28", NULL)],
        [2, 4, 7, "The Patent Troll",
         $ydb->YdbInstantParseIso8601("2017-06-04", NULL)],
        [2, 4, 8, "The Keenan Vortex",
         $ydb->YdbInstantParseIso8601("2017-06-11", NULL)],
        [2, 4, 9, "Hooli-Con", $ydb->YdbInstantParseIso8601("2017-06-18", NULL)],
        [2, 4, 10, "Server Error", $ydb->YdbInstantParseIso8601("2017-06-25", NULL)],
        [2, 5, 1, "Grow Fast or Die Slow",
         $ydb->YdbInstantParseIso8601("2018-03-25", NULL)],
        [2, 5, 2, "Reorientation", $ydb->YdbInstantParseIso8601("2018-04-01", NULL)],
        [2, 5, 3, "Chief Operating Officer",
         $ydb->YdbInstantParseIso8601("2018-04-08", NULL)],
        [2, 5, 4, "Tech Evangelist",
         $ydb->YdbInstantParseIso8601("2018-04-15", NULL)],
        [2, 5, 5, "Facial Recognition",
         $ydb->YdbInstantParseIso8601("2018-04-22", NULL)],
        [2, 5, 6, "Artificial Emotional Intelligence",
         $ydb->YdbInstantParseIso8601("2018-04-29", NULL)],
        [2, 5, 7, "Initial Coin Offering",
         $ydb->YdbInstantParseIso8601("2018-05-06", NULL)],
        [2, 5, 8, "Fifty-One Percent",
         $ydb->YdbInstantParseIso8601("2018-05-13", NULL)],
    ];

    $paramsBuilder = $ydb->YdbCreateParamsBuilder();

    $seriesParam =
        $ydb->YdbAddParam($paramsBuilder, "\$seriesData");
    $ydb->YdbParamBeginList($seriesParam);
    for ($i = 0; $i < count($seriesData); ++$i) {
        $ydb->YdbParamAddListItem($seriesParam);
        $ydb->YdbParamBeginStruct($seriesParam);
        $ydb->YdbParamAddMember($seriesParam, "series_id");
        $ydb->YdbParamUint64($seriesParam, $seriesData[$i][0]);
        $ydb->YdbParamAddMember($seriesParam, "title");
        $ydb->YdbParamUtf8($seriesParam, $seriesData[$i][1]);
        $ydb->YdbParamAddMember($seriesParam, "release_date");
        $ydb->YdbParamDate($seriesParam, $seriesData[$i][2]);
        $ydb->YdbParamAddMember($seriesParam, "series_info");
        $ydb->YdbParamUtf8($seriesParam, $seriesData[$i][3]);
        $ydb->YdbParamEndStruct($seriesParam);
    }
    $ydb->YdbParamEndList($seriesParam);
    $ydb->YdbBuildParamValue($seriesParam);

    $seasonsParam = $ydb->YdbAddParam($paramsBuilder, "\$seasonsData");
    $ydb->YdbParamBeginList($seasonsParam);
    for ($i = 0; $i < count($seasonsData); ++$i) {
        $ydb->YdbParamAddListItem($seasonsParam);
        $ydb->YdbParamBeginStruct($seasonsParam);
        $ydb->YdbParamAddMember($seasonsParam, "series_id");
        $ydb->YdbParamUint64($seasonsParam, $seasonsData[$i][0]);
        $ydb->YdbParamAddMember($seasonsParam, "season_id");
        $ydb->YdbParamUint64($seasonsParam, $seasonsData[$i][1]);
        $ydb->YdbParamAddMember($seasonsParam, "title");
        $ydb->YdbParamUtf8($seasonsParam, $seasonsData[$i][2]);
        $ydb->YdbParamAddMember($seasonsParam, "first_aired");
        $ydb->YdbParamDate($seasonsParam, $seasonsData[$i][3]);
        $ydb->YdbParamAddMember($seasonsParam, "last_aired");
        $ydb->YdbParamDate($seasonsParam, $seasonsData[$i][4]);
        $ydb->YdbParamEndStruct($seasonsParam);
    }
    $ydb->YdbParamEndList($seasonsParam);
    $ydb->YdbBuildParamValue($seasonsParam);

    $episodesParam = $ydb->YdbAddParam($paramsBuilder, "\$episodesData");
    $ydb->YdbParamBeginList($episodesParam);
    for ($i = 0; $i < count($episodesData); ++$i) {
        $ydb->YdbParamAddListItem($episodesParam);
        $ydb->YdbParamBeginStruct($episodesParam);
        $ydb->YdbParamAddMember($episodesParam, "series_id");
        $ydb->YdbParamUint64($episodesParam, $episodesData[$i][0]);
        $ydb->YdbParamAddMember($episodesParam, "season_id");
        $ydb->YdbParamUint64($episodesParam, $episodesData[$i][1]);
        $ydb->YdbParamAddMember($episodesParam, "episode_id");
        $ydb->YdbParamUint64($episodesParam, $episodesData[$i][2]);
        $ydb->YdbParamAddMember($episodesParam, "title");
        $ydb->YdbParamUtf8($episodesParam, $episodesData[$i][3]);
        $ydb->YdbParamAddMember($episodesParam, "air_date");
        $ydb->YdbParamDate($episodesParam, $episodesData[$i][4]);
        $ydb->YdbParamEndStruct($episodesParam);
    }
    $ydb->YdbParamEndList($episodesParam);
    $ydb->YdbBuildParamValue($episodesParam);

    return $ydb->YdbBuildParams($paramsBuilder);
}
