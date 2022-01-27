

#define LINK_TMP_NODE   do{                                         \
                                                                    \
                            if (ret_node == nullptr){               \
                                                                    \
                                ret_node = tmp_node;                \
                                cur_node = ret_node;                \
                            } else{                                 \
                                                                    \
                                assert(cur_node != nullptr);        \
                                cur_node->link(tmp_node);           \
                                cur_node = cur_node->get_next();    \
                            }                                       \
                        } while (0)