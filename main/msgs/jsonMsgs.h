#ifndef JSON_MESSAGES
#define JSON_MESSAGES


class jsonMsgs {
    public:
    string transmissionErrorNotAvaliable = "{type:\"transmission\",msg:\"No bytes avaliable to read following rec interupt\"}";
    string transmissionErrorOverflow = "{type:\"transmission\",msg:\"Overflow\"}";
    string transmissionErrorUnknownMsg = "{type:\"transmission\",msg:\"No message handler for msg\"}";
};


#endif