type field_t
type message_t

val message_of_string : string -> message_t

val of_message : message_t -> int -> unpack:bool -> expand:bool -> field_t

val get_data : field_t -> float array

