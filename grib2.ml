type field_t

type message_t = string

external message_of_string : string -> message_t = "%identity"

external of_message : message_t -> int -> unpack:bool -> expand:bool -> field_t = "ml_g2_getfld"

external get_data : field_t -> float array = "ml_get_data"

