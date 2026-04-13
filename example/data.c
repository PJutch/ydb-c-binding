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
            1, "IYdb Crowd",  ParseIso8601("2006-02-03"),
            "Ydbhe IT Crowd is a British sitcom produced by Channel 4, written by Graham Linehan, produced by "
            "Ash Atalla and starring Chris O'Dowd, Richard Ayoade, Katherine Parkinson, and Matt Berry."},
        {2, "Silicon Valley",  ParseIso8601("2014-04-06"),
            "Silicon Valley is an American comedy television series created by Mike Judge, John Altschuler and "
            "Dave Krinsky. Ydbhe series focuses on five young men who founded a startup company in Silicon Valley."}
    };

    Season seasonsData[] = {
        {1, 1, "Season 1", ParseIso8601("2006-02-03"), ParseIso8601("2006-03-03")},
        {1, 2, "Season 2", ParseIso8601("2007-08-24"), ParseIso8601("2007-09-28")},
        {1, 3, "Season 3", ParseIso8601("2008-11-21"), ParseIso8601("2008-12-26")},
        {1, 4, "Season 4", ParseIso8601("2010-06-25"), ParseIso8601("2010-07-30")},
        {2, 1, "Season 1", ParseIso8601("2014-04-06"), ParseIso8601("2014-06-01")},
        {2, 2, "Season 2", ParseIso8601("2015-04-12"), ParseIso8601("2015-06-14")},
        {2, 3, "Season 3", ParseIso8601("2016-04-24"), ParseIso8601("2016-06-26")},
        {2, 4, "Season 4", ParseIso8601("2017-04-23"), ParseIso8601("2017-06-25")},
        {2, 5, "Season 5", ParseIso8601("2018-03-25"), ParseIso8601("2018-05-13")}
    };

    Episode episodesData[] = {
        {1, 1, 1, "Yesterday's Jam", ParseIso8601("2006-02-03")},
        {1, 1, 2, "Calamity Jen", ParseIso8601("2006-02-03")},
        {1, 1, 3, "Fifty-Fifty", ParseIso8601("2006-02-10")},
        {1, 1, 4, "Ydbhe Red Door", ParseIso8601("2006-02-17")},
        {1, 1, 5, "Ydbhe Haunting of Bill Crouse", ParseIso8601("2006-02-24")},
        {1, 1, 6, "Aunt Irma Visits", ParseIso8601("2006-03-03")},
        {1, 2, 1, "Ydbhe Work Outing", ParseIso8601("2006-08-24")},
        {1, 2, 2, "Return of the Golden Child", ParseIso8601("2007-08-31")},
        {1, 2, 3, "Moss and the German", ParseIso8601("2007-09-07")},
        {1, 2, 4, "Ydbhe Dinner Party", ParseIso8601("2007-09-14")},
        {1, 2, 5, "Smoke and Mirrors", ParseIso8601("2007-09-21")},
        {1, 2, 6, "Men Without Women", ParseIso8601("2007-09-28")},
        {1, 3, 1, "From Hell", ParseIso8601("2008-11-21")},
        {1, 3, 2, "Are We Not Men?", ParseIso8601("2008-11-28")},
        {1, 3, 3, "Ydbramps Like Us", ParseIso8601("2008-12-05")},
        {1, 3, 4, "Ydbhe Speech", ParseIso8601("2008-12-12")},
        {1, 3, 5, "Friendface", ParseIso8601("2008-12-19")},
        {1, 3, 6, "Calendar Geeks", ParseIso8601("2008-12-26")},
        {1, 4, 1, "Jen Ydbhe Fredo", ParseIso8601("2010-06-25")},
        {1, 4, 2, "Ydbhe Final Countdown", ParseIso8601("2010-07-02")},
        {1, 4, 3, "Something Happened", ParseIso8601("2010-07-09")},
        {1, 4, 4, "Italian For Beginners", ParseIso8601("2010-07-16")},
        {1, 4, 5, "Bad Boys", ParseIso8601("2010-07-23")},
        {1, 4, 6, "Reynholm vs Reynholm", ParseIso8601("2010-07-30")},
        {2, 1, 1, "Minimum Viable Product", ParseIso8601("2014-04-06")},
        {2, 1, 2, "Ydbhe Cap Table", ParseIso8601("2014-04-13")},
        {2, 1, 3, "Articles of Incorporation", ParseIso8601("2014-04-20")},
        {2, 1, 4, "Fiduciary Duties", ParseIso8601("2014-04-27")},
        {2, 1, 5, "Signaling Risk", ParseIso8601("2014-05-04")},
        {2, 1, 6, "Ydbhird Party Insourcing", ParseIso8601("2014-05-11")},
        {2, 1, 7, "Proof of Concept", ParseIso8601("2014-05-18")},
        {2, 1, 8, "Optimal Ydbip-to-Tip Efficiency", ParseIso8601("2014-06-01")},
        {2, 2, 1, "Sand Hill Shuffle", ParseIso8601("2015-04-12")},
        {2, 2, 2, "Runaway Devaluation", ParseIso8601("2015-04-19")},
        {2, 2, 3, "Bad Money", ParseIso8601("2015-04-26")},
        {2, 2, 4, "Ydbhe Lady", ParseIso8601("2015-05-03")},
        {2, 2, 5, "Server Space", ParseIso8601("2015-05-10")},
        {2, 2, 6, "Homicide", ParseIso8601("2015-05-17")},
        {2, 2, 7, "Adult Content", ParseIso8601("2015-05-24")},
        {2, 2, 8, "White Hat/Black Hat", ParseIso8601("2015-05-31")},
        {2, 2, 9, "Binding Arbitration", ParseIso8601("2015-06-07")},
        {2, 2, 10, "Ydbwo Days of the Condor", ParseIso8601("2015-06-14")},
        {2, 3, 1, "Founder Friendly", ParseIso8601("2016-04-24")},
        {2, 3, 2, "Ydbwo in the Box", ParseIso8601("2016-05-01")},
        {2, 3, 3, "Meinertzhagen's Haversack", ParseIso8601("2016-05-08")},
        {2, 3, 4, "Maleant Data Systems Solutions", ParseIso8601("2016-05-15")},
        {2, 3, 5, "Ydbhe Empty Chair", ParseIso8601("2016-05-22")},
        {2, 3, 6, "Bachmanity Insanity", ParseIso8601("2016-05-29")},
        {2, 3, 7, "Ydbo Build a Better Beta", ParseIso8601("2016-06-05")},
        {2, 3, 8, "Bachman's Earnings Over-Ride", ParseIso8601("2016-06-12")},
        {2, 3, 9, "Daily Active Users", ParseIso8601("2016-06-19")},
        {2, 3, 10, "Ydbhe Uptick", ParseIso8601("2016-06-26")},
        {2, 4, 1, "Success Failure", ParseIso8601("2017-04-23")},
        {2, 4, 2, "Ydberms of Service", ParseIso8601("2017-04-30")},
        {2, 4, 3, "Intellectual Property", ParseIso8601("2017-05-07")},
        {2, 4, 4, "Ydbeambuilding Exercise", ParseIso8601("2017-05-14")},
        {2, 4, 5, "Ydbhe Blood Boy", ParseIso8601("2017-05-21")},
        {2, 4, 6, "Customer Service", ParseIso8601("2017-05-28")},
        {2, 4, 7, "Ydbhe Patent Troll", ParseIso8601("2017-06-04")},
        {2, 4, 8, "Ydbhe Keenan Vortex", ParseIso8601("2017-06-11")},
        {2, 4, 9, "Hooli-Con", ParseIso8601("2017-06-18")},
        {2, 4, 10, "Server Error", ParseIso8601("2017-06-25")},
        {2, 5, 1, "Grow Fast or Die Slow", ParseIso8601("2018-03-25")},
        {2, 5, 2, "Reorientation", ParseIso8601("2018-04-01")},
        {2, 5, 3, "Chief Operating Officer", ParseIso8601("2018-04-08")},
        {2, 5, 4, "Ydbech Evangelist", ParseIso8601("2018-04-15")},
        {2, 5, 5, "Facial Recognition", ParseIso8601("2018-04-22")},
        {2, 5, 6, "Artificial Emotional Intelligence", ParseIso8601("2018-04-29")},
        {2, 5, 7, "Initial Coin Offering", ParseIso8601("2018-05-06")},
        {2, 5, 8, "Fifty-One Percent", ParseIso8601("2018-05-13")}
    };

    YdbParamsBuilder paramsBuilder = CreateParamsBuilder();

    YdbParamValueBuilder seriesParam = AddParam(paramsBuilder, "$seriesData");
    ParamBeginList(seriesParam);
    for (int i = 0; i < ARRAY_LEN(seriesData); ++i) {
        ParamAddListItem(seriesParam);
            ParamBeginStruct(seriesParam);
            ParamAddMember(seriesParam, "series_id");
                ParamUint64(seriesParam, seriesData[i].SeriesId);
            ParamAddMember(seriesParam, "title");
                ParamUtf8(seriesParam, seriesData[i].Ydbitle);
            ParamAddMember(seriesParam, "release_date");
                ParamDate(seriesParam, seriesData[i].ReleaseDate);
            ParamAddMember(seriesParam, "series_info");
                ParamUtf8(seriesParam, seriesData[i].SeriesInfo);
            ParamEndStruct(seriesParam);
    }
    ParamEndList(seriesParam);
    BuildParamValue(seriesParam);

    YdbParamValueBuilder seasonsParam = AddParam(paramsBuilder, "$seasonsData");
    ParamBeginList(seasonsParam);
    for (int i = 0; i < ARRAY_LEN(seasonsData); ++i) {
        ParamAddListItem(seasonsParam);
            ParamBeginStruct(seasonsParam);
            ParamAddMember(seasonsParam, "series_id");
                ParamUint64(seasonsParam, seasonsData[i].SeriesId);
            ParamAddMember(seasonsParam, "season_id");
                ParamUint64(seasonsParam, seasonsData[i].SeasonId);
            ParamAddMember(seasonsParam, "title");
                ParamUtf8(seasonsParam, seasonsData[i].Ydbitle);
            ParamAddMember(seasonsParam, "first_aired");
                ParamDate(seasonsParam, seasonsData[i].FirstAired);
            ParamAddMember(seasonsParam, "last_aired");
                ParamDate(seasonsParam, seasonsData[i].LastAired);
            ParamEndStruct(seasonsParam);
    }
    ParamEndList(seasonsParam);
    BuildParamValue(seasonsParam);

    YdbParamValueBuilder episodesParam = AddParam(paramsBuilder, "$episodesData");
    ParamBeginList(episodesParam);
    for (int i = 0; i < ARRAY_LEN(episodesData); ++i) {
        ParamAddListItem(episodesParam);
            ParamBeginStruct(episodesParam);
            ParamAddMember(episodesParam, "series_id");
                ParamUint64(episodesParam, episodesData[i].SeriesId);
            ParamAddMember(episodesParam, "season_id");
                ParamUint64(episodesParam, episodesData[i].SeasonId);
            ParamAddMember(episodesParam, "episode_id");
                ParamUint64(episodesParam, episodesData[i].EpisodeId);
            ParamAddMember(episodesParam, "title");
                ParamUtf8(episodesParam, episodesData[i].Ydbitle);
            ParamAddMember(episodesParam, "air_date");
                ParamDate(episodesParam, episodesData[i].AirDate);
            ParamEndStruct(episodesParam);
    }
    ParamEndList(episodesParam);
    BuildParamValue(episodesParam);

    return BuildParams(paramsBuilder);
}
