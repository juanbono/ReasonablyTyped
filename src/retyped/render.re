let variableDeclaration ::name ::module_id ::type_of ::is_exports=false ::code="" () =>
  if is_exports {
    "external " ^ name ^ " : " ^ type_of ^ " = \"" ^ module_id ^ "\" [@@bs.module];\n"
  } else {
    "external " ^
    name ^ " : " ^ type_of ^ " = \"" ^ code ^ "\" [@@bs.module \"" ^ module_id ^ "\"];\n"
  };

let moduleDeclaration ::name ::statements () =>
  "module " ^ name ^ " = {\n" ^ String.concat "\n  " statements ^ "\n};";

let classDeclaration ::name ::exported_as ::module_id ::class_type ::ctor_type () =>
  "type " ^
  name ^
  " = " ^
  class_type ^
  ";\n" ^
  "external create_" ^
  name ^
  " : " ^ ctor_type ^ " = \"" ^ exported_as ^ "\" [@@bs.new] [@@bs.module \"" ^ module_id ^ "\"];";

let typeDeclaration ::name ::type_of () => "type " ^ name ^ " = " ^ type_of ^ ";";

let objectType ::statements () =>
  "Js.t {. " ^
  (
    List.filter (fun (key, type_of) => key != "__callProperty") statements |>
    List.map (fun (key, type_of) => key ^ ": " ^ type_of) |>
    String.concat ", "
  ) ^ " }";

let functionType ::params ::has_optional ::return_type () =>
  (
    List.map
      (
        fun (name, param_type) =>
          if (name != "") {
            name ^ "::" ^ param_type
          } else {
            param_type
          }
      )
      params |>
    String.concat " => "
  ) ^
  " => " ^ (has_optional ? "unit => " : "") ^ return_type;

let tupleType ::types () => "(" ^ String.concat ", " types ^ ")";

let unionTypeStrings ::types () =>
  "([" ^
  (List.map (fun type_name => "`" ^ type_name) types |> String.concat " | ") ^ "] [@bs.string])";

let unionType ::name ::types () =>
  "type union_of_" ^
  name ^
  " = " ^
  (
    List.map (fun (type_name, type_of) => "\n| " ^ type_name ^ " (" ^ type_of ^ ")") types |>
    String.concat ""
  ) ^
  ";\n\ntype " ^
  name ^
  ";\n\nexternal " ^
  name ^ " : union_of_" ^ name ^ " => " ^ name ^ " = \"Array.prototype.shift.call\" [@@bs.val];\n";

let classType ::types () =>
  "Js.t {. " ^
  (
    List.filter (fun (key, type_of, _) => key != "constructor") types |>
    List.map (fun (key, type_of, is_meth) => key ^ ": " ^ type_of ^ (is_meth ? "[@bs.meth]" : "")) |>
    String.concat ", "
  ) ^ "}";

let alias ::name ::value () => "let " ^ name ^ " = " ^ value ^ ";\n";
