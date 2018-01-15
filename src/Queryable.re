open SharedTypes;

type t 'a;

external _query : t 'a => string =>
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
