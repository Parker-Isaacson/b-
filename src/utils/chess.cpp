#include "chess.h"
#include <cctype>
#include <cstdlib>
#include <string>

std::vector<std::pair<int, int>> get_piece_moves(Piece piece) {
    switch (piece) {
        case Piece::White_King:
        case Piece::Black_King:
            return {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}}; // Does not include castling

        case Piece::White_Queen:
        case Piece::Black_Queen:
            return {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}, {2, 0}, {-2, 0}, {0, 2}, {0, -2}, {2, 2}, {-2, -2}, {2, -2}, {-2, 2}, {3, 0}, {-3, 0}, {0, 3}, {0, -3}, {3, 3}, {-3, -3}, {3, -3}, {-3, 3}, {4, 0}, {-4, 0}, {0, 4}, {0, -4}, {4, 4}, {-4, -4}, {4, -4}, {-4, 4}, {5, 0}, {-5, 0}, {0, 5}, {0, -5}, {5, 5}, {-5, -5}, {5, -5}, {-5, 5}, {6, 0}, {-6, 0}, {0, 6}, {0, -6}, {6, 6}, {-6, -6}, {6, -6}, {-6, 6}, {7, 0}, {-7, 0}, {0, 7}, {0, -7}, {7, 7}, {-7, -7}, {7, -7}, {-7, 7}};

        case Piece::White_Rook:
        case Piece::Black_Rook:
            return {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {2, 0}, {-2, 0}, {0, 2}, {0, -2}, {3, 0}, {-3, 0}, {0, 3}, {0, -3}, {4, 0}, {-4, 0}, {0, 4}, {0, -4}, {5, 0}, {-5, 0}, {0, 5}, {0, -5}, {6, 0}, {-6, 0}, {0, 6}, {0, -6}, {7, 0}, {-7, 0}, {0, 7}, {0, -7}};

        case Piece::White_Bishop:
        case Piece::Black_Bishop:
            return {{1, 1}, {-1, -1}, {1, -1}, {-1, 1}, {2, 2}, {-2, -2}, {2, -2}, {-2, 2}, {3, 3}, {-3, -3}, {3, -3}, {-3, 3}, {4, 4}, {-4, -4}, {4, -4}, {-4, 4}, {5, 5}, {-5, -5}, {5, -5}, {-5, 5}, {6, 6}, {-6, -6}, {6, -6}, {-6, 6}, {7, 7}, {-7, -7}, {7, -7}, {-7, 7}};

        case Piece::White_Knight:
        case Piece::Black_Knight:
            return {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {-1, 2}, {1, -2}, {-1, -2}};

        case Piece::White_Pawn:
        case Piece::Black_Pawn:
            return {{1, 0}}; // Does not include two moves forward, will be include if white is on the 2nd rank, or if black is on the 7th.

        case Piece::Empty:
        default:
            return {};
    }
}

char piece_to_string(Piece p) {
    switch (p) {
        case Piece::White_King:
            return 'K';

        case Piece::White_Queen:
            return 'Q';

        case Piece::White_Bishop:
            return 'B';

        case Piece::White_Knight:
            return 'N';

        case Piece::White_Rook:
            return 'R';

        case Piece::White_Pawn:
            return 'P';

        case Piece::Black_King:
            return 'k';

        case Piece::Black_Queen:
            return 'q';

        case Piece::Black_Bishop:
            return 'b';

        case Piece::Black_Knight:
            return 'n';

        case Piece::Black_Rook:
            return 'r';

        case Piece::Black_Pawn:
            return 'p';

        default:
            return 'z';
    }
}

Piece string_to_piece(char c) {
    switch (c) {
        case 'K':
            return Piece::White_King;

        case 'Q':
            return Piece::White_Queen;

        case 'B':
            return Piece::White_Bishop;

        case 'N':
            return Piece::White_Knight;

        case 'R':
            return Piece::White_Rook;

        case 'P':
            return Piece::White_Pawn;

        case 'k':
            return Piece::Black_King;

        case 'q':
            return Piece::Black_Queen;

        case 'b':
            return Piece::Black_Bishop;

        case 'n':
            return Piece::Black_Knight;

        case 'r':
            return Piece::Black_Rook;

        case 'p':
            return Piece::Black_Pawn;

        default:
            return Piece::Empty;
    }
}

Side Game::side_of_piece(Piece p) {
    switch (p) {
        case Piece::White_King:
        case Piece::White_Queen:
        case Piece::White_Bishop:
        case Piece::White_Knight:
        case Piece::White_Rook:
        case Piece::White_Pawn:
            return Side::White;

        case Piece::Black_King:
        case Piece::Black_Queen:
        case Piece::Black_Bishop:
        case Piece::Black_Knight:
        case Piece::Black_Rook:
        case Piece::Black_Pawn:
            return Side::Black;

  default:
            return Side::Empty;
    }
}

Game::Game() {
    board = {
        std::array<Piece, 8>{Piece::Black_Rook, Piece::Black_Knight, Piece::Black_Bishop, Piece::Black_Queen, Piece::Black_King, Piece::Black_Bishop, Piece::Black_Knight, Piece::Black_Rook},
        std::array<Piece, 8>{Piece::Black_Pawn, Piece::Black_Pawn, Piece::Black_Pawn, Piece::Black_Pawn, Piece::Black_Pawn, Piece::Black_Pawn, Piece::Black_Pawn, Piece::Black_Pawn},
        std::array<Piece, 8>{Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty},
        std::array<Piece, 8>{Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty},
        std::array<Piece, 8>{Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty},
        std::array<Piece, 8>{Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty, Piece::Empty},
        std::array<Piece, 8>{Piece::White_Pawn, Piece::White_Pawn, Piece::White_Pawn, Piece::White_Pawn, Piece::White_Pawn, Piece::White_Pawn, Piece::White_Pawn, Piece::White_Pawn},
        std::array<Piece, 8>{Piece::White_Rook, Piece::White_Knight, Piece::White_Bishop, Piece::White_Queen, Piece::White_King, Piece::White_Bishop, Piece::White_Knight, Piece::White_Rook},
    };

    check_moves();
}

Game::Game(std::string notation) { give_board_state(notation); }

std::string Game::get_board_state() {
    std::string state = "";

    int rankCount = 0;
    for (std::array<Piece, 8> rank : board) {
        int count = 0;
        for (Piece p : rank) {
            char m = piece_to_string(p);
            if ( m == 'z' ) {
                count++;
                continue;
            }

            if ( count > 0 ) {
                state += std::to_string(count);
                count = 0;
            }
            
            state += m;
        }
        
        if ( count > 0 )
            state += std::to_string(count);
        
        rankCount++;

        if ( rankCount != 8 )
            state += "/";
    }

    state += ( ToMove == Side::White) ? " w " : " b ";

    state += ( WhiteCastle ) ? "K" : "";
    state += ( WhiteLongCastle ) ? "Q" : "";
    state += ( BlackCastle ) ? "k" : "";
    state += ( BlackLongCastle ) ? "q" : "";

    state += ( ! (WhiteCastle || WhiteLongCastle || BlackCastle || BlackLongCastle ) ) ? "- " : " ";

    state += en_passant.to_string() + " ";

    state += std::to_string(halfMove) + " " + std::to_string(fullMove);

    return state;
}

void Game::give_board_state(std::string state) {
    ToMove = Side::White;

    WhiteCastle = false;
    WhiteLongCastle = false;
    BlackCastle = false;
    BlackLongCastle = false;

    en_passant = Square();

    halfMove = 0;
    fullMove = 1;

    board = Board{};
    
    size_t i = 0;

    { // To scope rank and file
        int rank = 0, file = 0;
        for (; i < state.length(); i++) {
            if ( state[i] == ' ' )
                break;

            if ( state[i] ==  '/' ) {
                rank += 1;
                file = 0;
                continue;
            }

            if (std::isdigit(state[i])) {
                int n = state[i] - '0'; // char "cast" to int
                for (int j = 0; j < n; j++) {
                    board[rank][file] = Piece::Empty;
                    file++;
                }
                continue;
            }

            board[rank][file] = string_to_piece(state[i]);
            file++;
        }
    }

    if ( state[++i] == 'b' )
        ToMove = Side::Black;

    i += 2;
    if ( state[i] != '-' ) {
        if ( state[i] == 'K' ) {
            WhiteCastle = true;
            ++i;
        }
        if ( state[i] == 'Q' ) {
            WhiteLongCastle = true;
            ++i;
        }
        if ( state[i] == 'k' ) {
            BlackCastle = true;
            ++i;
        }
        if ( state[i] == 'q' ) {
            BlackLongCastle = true;
            ++i;
        }
    }

    i += 1;
    if ( state[i] != '-' ) {
        en_passant = Square(state[i], state[i + 1]);
        ++i;
    }

    i += 2;
    int j = 0;
    for (; std::isdigit(state[i + j]); j++) { } // Find the length of the half move clock
    halfMove = std::stoi(state.substr(i, j));
    i += j + 1;

    j = 0;
    for (; std::isdigit(state[i + j]); j++) { } // Find the length of the full move clock
    fullMove = std::stoi(state.substr(i, j));

    check_moves();
}

// https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning#Pseudocode
Move Game::get_move() {
    auto evaluate = [&](Board b) { // Assume this works for know

    };

    auto alphabeta = [&](Board node, int depth, double alpha, double beta, bool maxPlayer) {

    };
    
    return Move(Square(0, 0), Square(0, 0)); // TODO: actually implemnet
}

bool Game::give_move(Move move) {
    return update_board(move);
}

bool Game::give_move(Square from, Square to, Piece promo) {
    return update_board(Move(from, to, promo));
}

bool Game::update_board(Move move) {
    for (const Move& m : moves) {
        bool same_squares = (move.from == m.from && move.to == m.to);
        bool promo_ok = (move.promotion == m.promotion) ||
                        (move.promotion == Piece::Empty && m.promotion != Piece::Empty);

        if (same_squares && promo_ok) {
            Move chosen = m;
            if (move.promotion != Piece::Empty)
                chosen.promotion = move.promotion;

            Piece moving = board[chosen.from.rank][chosen.from.file];
            Piece captured = board[chosen.to.rank][chosen.to.file];

            Square old_ep = en_passant;

            if (moving == Piece::White_Pawn || moving == Piece::Black_Pawn || captured != Piece::Empty)
                halfMove = 0;
            else
                ++halfMove;

            board[chosen.to.rank][chosen.to.file] = moving;
            board[chosen.from.rank][chosen.from.file] = Piece::Empty;

            if ((moving == Piece::White_Pawn || moving == Piece::Black_Pawn) &&
                captured == Piece::Empty &&
                old_ep.rank != -1 && old_ep.file != -1 &&
                chosen.to == old_ep &&
                std::abs(chosen.to.file - chosen.from.file) == 1) {
                int cap_rank = (moving == Piece::White_Pawn) ? (chosen.to.rank + 1) : (chosen.to.rank - 1);
                if (cap_rank >= 0 && cap_rank < 8)
                    board[cap_rank][chosen.to.file] = Piece::Empty;
            }

            en_passant = Square();

            if (moving == Piece::White_Pawn && chosen.from.rank == 6 && chosen.to.rank == 4) {
                en_passant = Square(5, chosen.from.file);
            } else if (moving == Piece::Black_Pawn && chosen.from.rank == 1 && chosen.to.rank == 3) {
                en_passant = Square(2, chosen.from.file);
            }

            if ((moving == Piece::White_King || moving == Piece::Black_King) &&
                chosen.from.rank == chosen.to.rank &&
                std::abs(chosen.to.file - chosen.from.file) == 2) {
                int rank = chosen.from.rank;
                if (chosen.to.file == 6) {
                    Piece rook = board[rank][7];
                    board[rank][5] = rook;
                    board[rank][7] = Piece::Empty;
                }
                if (chosen.to.file == 2) {
                    Piece rook = board[rank][0];
                    board[rank][3] = rook;
                    board[rank][0] = Piece::Empty;
                }
            }

            if (moving == Piece::White_Pawn && chosen.to.rank == 0) {
                Piece promo = chosen.promotion;
                if (promo == Piece::Empty) promo = Piece::White_Queen;
                if (side_of_piece(promo) != Side::White) promo = Piece::White_Queen;
                board[chosen.to.rank][chosen.to.file] = promo;
            } else if (moving == Piece::Black_Pawn && chosen.to.rank == 7) {
                Piece promo = chosen.promotion;
                if (promo == Piece::Empty) promo = Piece::Black_Queen;
                if (side_of_piece(promo) != Side::Black) promo = Piece::Black_Queen;
                board[chosen.to.rank][chosen.to.file] = promo;
            }

            if (moving == Piece::White_King) {
                WhiteCastle = false;
                WhiteLongCastle = false;
            } else if (moving == Piece::Black_King) {
                BlackCastle = false;
                BlackLongCastle = false;
            } else if (moving == Piece::White_Rook) {
                if (chosen.from.rank == 7 && chosen.from.file == 0) WhiteLongCastle = false;
                if (chosen.from.rank == 7 && chosen.from.file == 7) WhiteCastle = false;
            } else if (moving == Piece::Black_Rook) {
                if (chosen.from.rank == 0 && chosen.from.file == 0) BlackLongCastle = false;
                if (chosen.from.rank == 0 && chosen.from.file == 7) BlackCastle = false;
            }

            if (captured == Piece::White_Rook) {
                if (chosen.to.rank == 7 && chosen.to.file == 0) WhiteLongCastle = false;
                if (chosen.to.rank == 7 && chosen.to.file == 7) WhiteCastle = false;
            } else if (captured == Piece::Black_Rook) {
                if (chosen.to.rank == 0 && chosen.to.file == 0) BlackLongCastle = false;
                if (chosen.to.rank == 0 && chosen.to.file == 7) BlackCastle = false;
            }

            if (ToMove == Side::Black)
                ++fullMove;
            ToMove = (ToMove == Side::White) ? Side::Black : Side::White;

            return check_moves();
        }
    }
    return false;
}


std::vector<Move> Game::children(const Board& board, const PositionState& st) {
    std::vector<Move> legal;
    std::vector<Move> pseudo;

    auto in_bounds = [](int r, int f) {
        return r >= 0 && r < 8 && f >= 0 && f < 8;
    };

    auto opponent = [&](Side s) {
        if (s == Side::White) return Side::Black;
        if (s == Side::Black) return Side::White;
        return Side::Empty;
    };

    auto is_enemy = [&](Piece p) {
        Side s = side_of_piece(p);
        return s != Side::Empty && s != st.toMove;
    };

    auto add_if_ok = [&](int fr, int ff, int tr, int tf) {
        if (!in_bounds(tr, tf)) return;
        Piece target = board[tr][tf];
        if (side_of_piece(target) == st.toMove) return;
        pseudo.emplace_back(fr, ff, tr, tf);
    };

    auto pin_ray = [&](int fr, int ff, int dr, int df) {
        for (int step = 1; step < 8; ++step) {
            int tr = fr + dr * step;
            int tf = ff + df * step;
            if (!in_bounds(tr, tf)) break;

            Piece target = board[tr][tf];
            if (target == Piece::Empty) {
                pseudo.emplace_back(fr, ff, tr, tf);
                continue;
            }

            if (side_of_piece(target) != st.toMove) {
                pseudo.emplace_back(fr, ff, tr, tf);
            }
            break;
        }
    };

    auto attacked_by_board = [&](const Board& b, Side attacker, int sr, int sf) {
        auto inb = [&](int r, int f) { return r >= 0 && r < 8 && f >= 0 && f < 8; };

        // Pawn attacks
        if (attacker == Side::White) {
            int pr = sr + 1;
            if (pr < 8) {
                if (sf - 1 >= 0 && b[pr][sf - 1] == Piece::White_Pawn) return true;
                if (sf + 1 < 8 && b[pr][sf + 1] == Piece::White_Pawn) return true;
            }
        } else if (attacker == Side::Black) {
            int pr = sr - 1;
            if (pr >= 0) {
                if (sf - 1 >= 0 && b[pr][sf - 1] == Piece::Black_Pawn) return true;
                if (sf + 1 < 8 && b[pr][sf + 1] == Piece::Black_Pawn) return true;
            }
        }

        // Knight attacks
        static const int kdr[8] = {2, 2, -2, -2, 1, 1, -1, -1};
        static const int kdf[8] = {1, -1, 1, -1, 2, -2, 2, -2};
        for (int i = 0; i < 8; ++i) {
            int r = sr + kdr[i];
            int f = sf + kdf[i];
            if (!inb(r, f)) continue;
            Piece p = b[r][f];
            if (attacker == Side::White && p == Piece::White_Knight) return true;
            if (attacker == Side::Black && p == Piece::Black_Knight) return true;
        }

        // King attacks
        for (int dr = -1; dr <= 1; ++dr) {
            for (int df = -1; df <= 1; ++df) {
                if (dr == 0 && df == 0) continue;
                int r = sr + dr;
                int f = sf + df;
                if (!inb(r, f)) continue;
                Piece p = b[r][f];
                if (attacker == Side::White && p == Piece::White_King) return true;
                if (attacker == Side::Black && p == Piece::Black_King) return true;
            }
        }

        // Sliding attacks
        auto ray_hits = [&](int dr, int df, bool rook_like, bool bishop_like) {
            for (int step = 1; step < 8; ++step) {
                int r = sr + dr * step;
                int f = sf + df * step;
                if (!inb(r, f)) return false;
                Piece p = b[r][f];
                if (p == Piece::Empty) continue;
                if (attacker == Side::White) {
                    if (rook_like && (p == Piece::White_Rook || p == Piece::White_Queen)) return true;
                    if (bishop_like && (p == Piece::White_Bishop || p == Piece::White_Queen)) return true;
                } else {
                    if (rook_like && (p == Piece::Black_Rook || p == Piece::Black_Queen)) return true;
                    if (bishop_like && (p == Piece::Black_Bishop || p == Piece::Black_Queen)) return true;
                }
                return false;
            }
            return false;
        };

        if (ray_hits(1, 0, true, false)) return true;
        if (ray_hits(-1, 0, true, false)) return true;
        if (ray_hits(0, 1, true, false)) return true;
        if (ray_hits(0, -1, true, false)) return true;
        if (ray_hits(1, 1, false, true)) return true;
        if (ray_hits(1, -1, false, true)) return true;
        if (ray_hits(-1, 1, false, true)) return true;
        if (ray_hits(-1, -1, false, true)) return true;

        return false;
    };

    auto find_king = [&](const Board& b, Side s, int& kr, int& kf) {
        Piece king = (s == Side::White) ? Piece::White_King : Piece::Black_King;
        for (int r = 0; r < 8; ++r)
            for (int f = 0; f < 8; ++f)
                if (b[r][f] == king) { kr = r; kf = f; return true; }
        return false;
    };

    auto apply_move_on = [&](Board& b, const Move& m) {
        Piece moving   = b[m.from.rank][m.from.file];
        Piece captured = b[m.to.rank][m.to.file];

        b[m.to.rank][m.to.file] = moving;
        b[m.from.rank][m.from.file] = Piece::Empty;

        // en-passant capture (use st.enPassant, not Game::en_passant)
        if ((moving == Piece::White_Pawn || moving == Piece::Black_Pawn) &&
            captured == Piece::Empty &&
            st.enPassant.is_valid() &&
            m.to == st.enPassant &&
            std::abs(m.to.file - m.from.file) == 1) {
            int cap_rank = (moving == Piece::White_Pawn) ? (m.to.rank + 1) : (m.to.rank - 1);
            if (cap_rank >= 0 && cap_rank < 8)
                b[cap_rank][m.to.file] = Piece::Empty;
        }

        // castling rook move
        if ((moving == Piece::White_King || moving == Piece::Black_King) &&
            m.from.rank == m.to.rank &&
            std::abs(m.to.file - m.from.file) == 2) {
            int rank = m.from.rank;
            if (m.to.file == 6) { // king-side
                b[rank][5] = b[rank][7];
                b[rank][7] = Piece::Empty;
            } else if (m.to.file == 2) { // queen-side
                b[rank][3] = b[rank][0];
                b[rank][0] = Piece::Empty;
            }
        }

        // promotion
        if (moving == Piece::White_Pawn && m.to.rank == 0) {
            Piece promo = (m.promotion == Piece::Empty) ? Piece::White_Queen : m.promotion;
            if (side_of_piece(promo) != Side::White) promo = Piece::White_Queen;
            b[m.to.rank][m.to.file] = promo;
        } else if (moving == Piece::Black_Pawn && m.to.rank == 7) {
            Piece promo = (m.promotion == Piece::Empty) ? Piece::Black_Queen : m.promotion;
            if (side_of_piece(promo) != Side::Black) promo = Piece::Black_Queen;
            b[m.to.rank][m.to.file] = promo;
        }
    };

    for (int r = 0; r < 8; ++r) {
        for (int f = 0; f < 8; ++f) {
            Piece p = board[r][f];
            if (side_of_piece(p) != st.toMove) continue;

            if (p == Piece::White_Pawn || p == Piece::Black_Pawn) {
                int dir        = (p == Piece::White_Pawn) ? -1 : 1;
                int start_rank = (p == Piece::White_Pawn) ? 6 : 1;
                int promo_rank = (p == Piece::White_Pawn) ? 0 : 7;

                auto add_pawn_move = [&](int fr, int ff, int tr, int tf) {
                    if (tr == promo_rank) {
                        if (st.toMove == Side::White) {
                            pseudo.emplace_back(fr, ff, tr, tf, Piece::White_Queen);
                            pseudo.emplace_back(fr, ff, tr, tf, Piece::White_Rook);
                            pseudo.emplace_back(fr, ff, tr, tf, Piece::White_Bishop);
                            pseudo.emplace_back(fr, ff, tr, tf, Piece::White_Knight);
                        } else {
                            pseudo.emplace_back(fr, ff, tr, tf, Piece::Black_Queen);
                            pseudo.emplace_back(fr, ff, tr, tf, Piece::Black_Rook);
                            pseudo.emplace_back(fr, ff, tr, tf, Piece::Black_Bishop);
                            pseudo.emplace_back(fr, ff, tr, tf, Piece::Black_Knight);
                        }
                    } else {
                        pseudo.emplace_back(fr, ff, tr, tf);
                    }
                };

                int r1 = r + dir;
                if (in_bounds(r1, f) && board[r1][f] == Piece::Empty) {
                    add_pawn_move(r, f, r1, f);

                    int r2 = r + 2 * dir;
                    if (r == start_rank && in_bounds(r2, f) && board[r2][f] == Piece::Empty) {
                        pseudo.emplace_back(r, f, r2, f);
                    }
                }

                for (int df : {-1, 1}) {
                    int tf = f + df;
                    int tr = r + dir;
                    if (!in_bounds(tr, tf)) continue;

                    Piece target = board[tr][tf];
                    if (is_enemy(target)) add_pawn_move(r, f, tr, tf);

                    if (st.enPassant.rank != -1 && st.enPassant.file != -1 &&
                        st.enPassant.rank == tr && st.enPassant.file == tf) {
                        pseudo.emplace_back(r, f, tr, tf);
                    }
                }

                continue;
            }

            if (p == Piece::White_Rook || p == Piece::Black_Rook) {
                pin_ray(r, f, 1, 0); pin_ray(r, f, -1, 0);
                pin_ray(r, f, 0, 1); pin_ray(r, f, 0, -1);
                continue;
            }
            if (p == Piece::White_Bishop || p == Piece::Black_Bishop) {
                pin_ray(r, f, 1, 1); pin_ray(r, f, 1, -1);
                pin_ray(r, f, -1, 1); pin_ray(r, f, -1, -1);
                continue;
            }
            if (p == Piece::White_Queen || p == Piece::Black_Queen) {
                pin_ray(r, f, 1, 0); pin_ray(r, f, -1, 0);
                pin_ray(r, f, 0, 1); pin_ray(r, f, 0, -1);
                pin_ray(r, f, 1, 1); pin_ray(r, f, 1, -1);
                pin_ray(r, f, -1, 1); pin_ray(r, f, -1, -1);
                continue;
            }

            if (p == Piece::White_King || p == Piece::Black_King) {
                for (auto [dr, df] : get_piece_moves(p))
                    add_if_ok(r, f, r + dr, f + df);

                Side opp = opponent(st.toMove);

                if (st.toMove == Side::White && r == 7 && f == 4) {
                    if (st.castle.whiteKingSide &&
                        board[7][7] == Piece::White_Rook &&
                        board[7][5] == Piece::Empty && board[7][6] == Piece::Empty &&
                        !attacked_by_board(board, opp, 7, 4) &&
                        !attacked_by_board(board, opp, 7, 5) &&
                        !attacked_by_board(board, opp, 7, 6)) {
                        pseudo.emplace_back(7, 4, 7, 6);
                    }
                    if (st.castle.whiteQueenSide &&
                        board[7][0] == Piece::White_Rook &&
                        board[7][1] == Piece::Empty && board[7][2] == Piece::Empty && board[7][3] == Piece::Empty &&
                        !attacked_by_board(board, opp, 7, 4) &&
                        !attacked_by_board(board, opp, 7, 3) &&
                        !attacked_by_board(board, opp, 7, 2)) {
                        pseudo.emplace_back(7, 4, 7, 2);
                    }
                }

                if (st.toMove == Side::Black && r == 0 && f == 4) {
                    if (st.castle.blackKingSide &&
                        board[0][7] == Piece::Black_Rook &&
                        board[0][5] == Piece::Empty && board[0][6] == Piece::Empty &&
                        !attacked_by_board(board, opp, 0, 4) &&
                        !attacked_by_board(board, opp, 0, 5) &&
                        !attacked_by_board(board, opp, 0, 6)) {
                        pseudo.emplace_back(0, 4, 0, 6);
                    }
                    if (st.castle.blackQueenSide &&
                        board[0][0] == Piece::Black_Rook &&
                        board[0][1] == Piece::Empty && board[0][2] == Piece::Empty && board[0][3] == Piece::Empty &&
                        !attacked_by_board(board, opp, 0, 4) &&
                        !attacked_by_board(board, opp, 0, 3) &&
                        !attacked_by_board(board, opp, 0, 2)) {
                        pseudo.emplace_back(0, 4, 0, 2);
                    }
                }

                continue;
            }

            // Knight (or any piece using jump table)
            for (auto [dr, df] : get_piece_moves(p))
                add_if_ok(r, f, r + dr, f + df);
        }
    }

    Side opp = opponent(st.toMove);

    for (const Move& m : pseudo) {
        Board b = board;
        apply_move_on(b, m);

        int kr = -1, kf = -1;
        if (!find_king(b, st.toMove, kr, kf))
            continue;

        if (attacked_by_board(b, opp, kr, kf))
            continue;

        legal.push_back(m);
    }

    return legal;
}

bool Game::check_moves() {
    moves.clear();

    moves = children(board, PositionState(ToMove, en_passant, CastlingRights(WhiteCastle, WhiteLongCastle, BlackCastle, BlackLongCastle)));
    return true;
}

std::string Game::print_moves() {
    std::string ret = get_board_state() + "\nMoves: " + std::to_string(moves.size()) + "\t To Move: " + ((ToMove == Side::White) ? "White" : "Black") + "\n";

    for (Move m : moves)
        ret += m.to_string() + "\n";

    return ret;
}

Side Game::checkmate() {
    check_moves();
    if (moves.empty())
        return (ToMove == Side::White) ? Side::Black : Side::White;
    return Side::Empty;
}
