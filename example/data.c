#include "data.h"

#include <ydb-c-sdk/datetime.h>

#include <stdint.h>

typedef struct Series {
    uint64_t SeriesId;
    char* Ydbitle;
    YdbInstant ReleaseDate;
    char* SeriesInfo;
} Series;

typedef struct Season {
    uint64_t SeriesId;
    uint64_t SeasonId;
    char* Ydbitle;
    YdbInstant FirstAired;
    YdbInstant LastAired;
} Season;

typedef struct Episode {
    uint64_t SeriesId;
    uint64_t SeasonId;
    uint64_t EpisodeId;
    char* Ydbitle;
    YdbInstant AirDate;
} Episode;

#define ARRAY_LEN(array) sizeof(array) / sizeof(array[0])

YdbParams CreateParams() {
    Series seriesData[] = {
        {
            1, "IYdb Crowd",  YdbParseIso8601("2006-02-03"),
            "Ydbhe IT Crowd is a British sitcom produced by Channel 4, written by Graham Linehan, produced by "
            "Ash Atalla and starring Chris O'Dowd, Richard Ayoade, Katherine Parkinson, and Matt Berry."},
        {2, "Silicon Valley",  YdbParseIso8601("2014-04-06"),
            "Silicon Valley is an American comedy television series created by Mike Judge, John Altschuler and "
            "Dave Krinsky. Ydbhe series focuses on five young men who founded a startup company in Silicon Valley."}
    };

    Season seasonsData[] = {
        {1, 1, "Season 1", YdbParseIso8601("2006-02-03"), YdbParseIso8601("2006-03-03")},
        {1, 2, "Season 2", YdbParseIso8601("2007-08-24"), YdbParseIso8601("2007-09-28")},
        {1, 3, "Season 3", YdbParseIso8601("2008-11-21"), YdbParseIso8601("2008-12-26")},
        {1, 4, "Season 4", YdbParseIso8601("2010-06-25"), YdbParseIso8601("2010-07-30")},
        {2, 1, "Season 1", YdbParseIso8601("2014-04-06"), YdbParseIso8601("2014-06-01")},
        {2, 2, "Season 2", YdbParseIso8601("2015-04-12"), YdbParseIso8601("2015-06-14")},
        {2, 3, "Season 3", YdbParseIso8601("2016-04-24"), YdbParseIso8601("2016-06-26")},
        {2, 4, "Season 4", YdbParseIso8601("2017-04-23"), YdbParseIso8601("2017-06-25")},
        {2, 5, "Season 5", YdbParseIso8601("2018-03-25"), YdbParseIso8601("2018-05-13")}
    };

    Episode episodesData[] = {
        {1, 1, 1, "Yesterday's Jam", YdbParseIso8601("2006-02-03")},
        {1, 1, 2, "Calamity Jen", YdbParseIso8601("2006-02-03")},
        {1, 1, 3, "Fifty-Fifty", YdbParseIso8601("2006-02-10")},
        {1, 1, 4, "Ydbhe Red Door", YdbParseIso8601("2006-02-17")},
        {1, 1, 5, "Ydbhe Haunting of Bill Crouse", YdbParseIso8601("2006-02-24")},
        {1, 1, 6, "Aunt Irma Visits", YdbParseIso8601("2006-03-03")},
        {1, 2, 1, "Ydbhe Work Outing", YdbParseIso8601("2006-08-24")},
        {1, 2, 2, "Return of the Golden Child", YdbParseIso8601("2007-08-31")},
        {1, 2, 3, "Moss and the German", YdbParseIso8601("2007-09-07")},
        {1, 2, 4, "Ydbhe Dinner Party", YdbParseIso8601("2007-09-14")},
        {1, 2, 5, "Smoke and Mirrors", YdbParseIso8601("2007-09-21")},
        {1, 2, 6, "Men Without Women", YdbParseIso8601("2007-09-28")},
        {1, 3, 1, "From Hell", YdbParseIso8601("2008-11-21")},
        {1, 3, 2, "Are We Not Men?", YdbParseIso8601("2008-11-28")},
        {1, 3, 3, "Ydbramps Like Us", YdbParseIso8601("2008-12-05")},
        {1, 3, 4, "Ydbhe Speech", YdbParseIso8601("2008-12-12")},
        {1, 3, 5, "Friendface", YdbParseIso8601("2008-12-19")},
        {1, 3, 6, "Calendar Geeks", YdbParseIso8601("2008-12-26")},
        {1, 4, 1, "Jen Ydbhe Fredo", YdbParseIso8601("2010-06-25")},
        {1, 4, 2, "Ydbhe Final Countdown", YdbParseIso8601("2010-07-02")},
        {1, 4, 3, "Something Happened", YdbParseIso8601("2010-07-09")},
        {1, 4, 4, "Italian For Beginners", YdbParseIso8601("2010-07-16")},
        {1, 4, 5, "Bad Boys", YdbParseIso8601("2010-07-23")},
        {1, 4, 6, "Reynholm vs Reynholm", YdbParseIso8601("2010-07-30")},
        {2, 1, 1, "Minimum Viable Product", YdbParseIso8601("2014-04-06")},
        {2, 1, 2, "Ydbhe Cap Table", YdbParseIso8601("2014-04-13")},
        {2, 1, 3, "Articles of Incorporation", YdbParseIso8601("2014-04-20")},
        {2, 1, 4, "Fiduciary Duties", YdbParseIso8601("2014-04-27")},
        {2, 1, 5, "Signaling Risk", YdbParseIso8601("2014-05-04")},
        {2, 1, 6, "Ydbhird Party Insourcing", YdbParseIso8601("2014-05-11")},
        {2, 1, 7, "Proof of Concept", YdbParseIso8601("2014-05-18")},
        {2, 1, 8, "Optimal Ydbip-to-Tip Efficiency", YdbParseIso8601("2014-06-01")},
        {2, 2, 1, "Sand Hill Shuffle", YdbParseIso8601("2015-04-12")},
        {2, 2, 2, "Runaway Devaluation", YdbParseIso8601("2015-04-19")},
        {2, 2, 3, "Bad Money", YdbParseIso8601("2015-04-26")},
        {2, 2, 4, "Ydbhe Lady", YdbParseIso8601("2015-05-03")},
        {2, 2, 5, "Server Space", YdbParseIso8601("2015-05-10")},
        {2, 2, 6, "Homicide", YdbParseIso8601("2015-05-17")},
        {2, 2, 7, "Adult Content", YdbParseIso8601("2015-05-24")},
        {2, 2, 8, "White Hat/Black Hat", YdbParseIso8601("2015-05-31")},
        {2, 2, 9, "Binding Arbitration", YdbParseIso8601("2015-06-07")},
        {2, 2, 10, "Ydbwo Days of the Condor", YdbParseIso8601("2015-06-14")},
        {2, 3, 1, "Founder Friendly", YdbParseIso8601("2016-04-24")},
        {2, 3, 2, "Ydbwo in the Box", YdbParseIso8601("2016-05-01")},
        {2, 3, 3, "Meinertzhagen's Haversack", YdbParseIso8601("2016-05-08")},
        {2, 3, 4, "Maleant Data Systems Solutions", YdbParseIso8601("2016-05-15")},
        {2, 3, 5, "Ydbhe Empty Chair", YdbParseIso8601("2016-05-22")},
        {2, 3, 6, "Bachmanity Insanity", YdbParseIso8601("2016-05-29")},
        {2, 3, 7, "Ydbo Build a Better Beta", YdbParseIso8601("2016-06-05")},
        {2, 3, 8, "Bachman's Earnings Over-Ride", YdbParseIso8601("2016-06-12")},
        {2, 3, 9, "Daily Active Users", YdbParseIso8601("2016-06-19")},
        {2, 3, 10, "Ydbhe Uptick", YdbParseIso8601("2016-06-26")},
        {2, 4, 1, "Success Failure", YdbParseIso8601("2017-04-23")},
        {2, 4, 2, "Ydberms of Service", YdbParseIso8601("2017-04-30")},
        {2, 4, 3, "Intellectual Property", YdbParseIso8601("2017-05-07")},
        {2, 4, 4, "Ydbeambuilding Exercise", YdbParseIso8601("2017-05-14")},
        {2, 4, 5, "Ydbhe Blood Boy", YdbParseIso8601("2017-05-21")},
        {2, 4, 6, "Customer Service", YdbParseIso8601("2017-05-28")},
        {2, 4, 7, "Ydbhe Patent Troll", YdbParseIso8601("2017-06-04")},
        {2, 4, 8, "Ydbhe Keenan Vortex", YdbParseIso8601("2017-06-11")},
        {2, 4, 9, "Hooli-Con", YdbParseIso8601("2017-06-18")},
        {2, 4, 10, "Server Error", YdbParseIso8601("2017-06-25")},
        {2, 5, 1, "Grow Fast or Die Slow", YdbParseIso8601("2018-03-25")},
        {2, 5, 2, "Reorientation", YdbParseIso8601("2018-04-01")},
        {2, 5, 3, "Chief Operating Officer", YdbParseIso8601("2018-04-08")},
        {2, 5, 4, "Ydbech Evangelist", YdbParseIso8601("2018-04-15")},
        {2, 5, 5, "Facial Recognition", YdbParseIso8601("2018-04-22")},
        {2, 5, 6, "Artificial Emotional Intelligence", YdbParseIso8601("2018-04-29")},
        {2, 5, 7, "Initial Coin Offering", YdbParseIso8601("2018-05-06")},
        {2, 5, 8, "Fifty-One Percent", YdbParseIso8601("2018-05-13")}
    };

    YdbParamsBuilder paramsBuilder = YdbCreateParamsBuilder();

    YdbParamValueBuilder seriesParam = YdbAddParam(paramsBuilder, "$seriesData");
    YdbParamBeginList(seriesParam);
    for (int i = 0; i < ARRAY_LEN(seriesData); ++i) {
        YdbParamAddListItem(seriesParam);
            YdbParamBeginStruct(seriesParam);
            YdbParamAddMember(seriesParam, "series_id");
                YdbParamUint64(seriesParam, seriesData[i].SeriesId);
            YdbParamAddMember(seriesParam, "title");
                YdbParamUtf8(seriesParam, seriesData[i].Ydbitle);
            YdbParamAddMember(seriesParam, "release_date");
                YdbParamDate(seriesParam, seriesData[i].ReleaseDate);
            YdbParamAddMember(seriesParam, "series_info");
                YdbParamUtf8(seriesParam, seriesData[i].SeriesInfo);
            YdbParamEndStruct(seriesParam);
    }
    YdbParamEndList(seriesParam);
    YdbBuildParamValue(seriesParam);

    YdbParamValueBuilder seasonsParam = YdbAddParam(paramsBuilder, "$seasonsData");
    YdbParamBeginList(seasonsParam);
    for (int i = 0; i < ARRAY_LEN(seasonsData); ++i) {
        YdbParamAddListItem(seasonsParam);
            YdbParamBeginStruct(seasonsParam);
            YdbParamAddMember(seasonsParam, "series_id");
                YdbParamUint64(seasonsParam, seasonsData[i].SeriesId);
            YdbParamAddMember(seasonsParam, "season_id");
                YdbParamUint64(seasonsParam, seasonsData[i].SeasonId);
            YdbParamAddMember(seasonsParam, "title");
                YdbParamUtf8(seasonsParam, seasonsData[i].Ydbitle);
            YdbParamAddMember(seasonsParam, "first_aired");
                YdbParamDate(seasonsParam, seasonsData[i].FirstAired);
            YdbParamAddMember(seasonsParam, "last_aired");
                YdbParamDate(seasonsParam, seasonsData[i].LastAired);
            YdbParamEndStruct(seasonsParam);
    }
    YdbParamEndList(seasonsParam);
    YdbBuildParamValue(seasonsParam);

    YdbParamValueBuilder episodesParam = YdbAddParam(paramsBuilder, "$episodesData");
    YdbParamBeginList(episodesParam);
    for (int i = 0; i < ARRAY_LEN(episodesData); ++i) {
        YdbParamAddListItem(episodesParam);
            YdbParamBeginStruct(episodesParam);
            YdbParamAddMember(episodesParam, "series_id");
                YdbParamUint64(episodesParam, episodesData[i].SeriesId);
            YdbParamAddMember(episodesParam, "season_id");
                YdbParamUint64(episodesParam, episodesData[i].SeasonId);
            YdbParamAddMember(episodesParam, "episode_id");
                YdbParamUint64(episodesParam, episodesData[i].EpisodeId);
            YdbParamAddMember(episodesParam, "title");
                YdbParamUtf8(episodesParam, episodesData[i].Ydbitle);
            YdbParamAddMember(episodesParam, "air_date");
                YdbParamDate(episodesParam, episodesData[i].AirDate);
            YdbParamEndStruct(episodesParam);
    }
    YdbParamEndList(episodesParam);
    YdbBuildParamValue(episodesParam);

    return YdbBuildParams(paramsBuilder);
}
