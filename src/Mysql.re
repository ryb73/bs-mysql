type connectionOptions;

external _connectionOptions : host::string? => port::int? => localAddress::string? =>
                                socketPath::string? => database::string? => charset::string? =>
                                timezone::string? => connectTimeout::int? =>
                                insecureAuth::Js.boolean? => supportBigNumbers::Js.boolean? =>
                                debug::Js.boolean? => trace::Js.boolean? => user::string? =>
                                password::string? => unit => connectionOptions = "" [@@bs.obj];

let connectionOptions ::host=? ::port=? ::localAddress=? ::socketPath=? ::database=? ::charset=?
    ::timezone=? ::connectTimeout=? ::insecureAuth=? ::supportBigNumbers=? ::debug=? ::trace=? ::user=?
    ::password=? () =>
{
    _connectionOptions ::?host ::?port ::?localAddress ::?socketPath ::?database ::?charset
        ::?timezone ::?connectTimeout ::?user ::?password
        insecureAuth::?(Option.map Js.Boolean.to_js_boolean insecureAuth)
        supportBigNumbers::?(Option.map Js.Boolean.to_js_boolean supportBigNumbers)
        debug::?(Option.map Js.Boolean.to_js_boolean debug)
        trace::?(Option.map Js.Boolean.to_js_boolean trace)
        ();
};

type t;
external createConnection : connectionOptions => t = "" [@@bs.module "mysql"];

exception JsonException Js.Json.t;
external _end : t => (Js.nullable Js.Json.t => unit) => unit = "end" [@@bs.send];
let end_ connection => {
    Js.Promise.make (fun ::resolve ::reject => {
        _end connection (fun err => {
            switch (Js.toOption err) {
                | None => let u = (); resolve u [@bs]
                | Some err => reject (JsonException err) [@bs]
            };
        });
    });
};

external destroy : t => unit = "" [@@bs.send];

external _query : t => string =>
    (Js.nullable Js.Json.t => Js.nullable Js.Json.t => Js.nullable Js.Json.t => unit)
    => unit = "query" [@@bs.send];
let query connection query => {
    Js.Promise.make (fun ::resolve ::reject => {
        _query connection query (fun err result fields => {
            switch (Js.toOption err) {
                | Some err => reject (JsonException err) [@bs]
                | None => resolve (
                    Js.Nullable.to_opt result |> Option.get,
                    Js.Nullable.to_opt fields
                ) [@bs]
            };
        });
    });
};