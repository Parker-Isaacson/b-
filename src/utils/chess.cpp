#include "chess.h"

std::vector<std::pair<int, int>> get_piece_moves(Piece piece) {
    switch (piece) {
        case Piece::White_King: case Piece::Black_King: return {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}}; // Does not include castling
        case Piece::White_Queen: case Piece::Black_Queen: return {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}, {2, 0}, {-2, 0}, {0, 2}, {0, -2}, {2, 2}, {-2, -2}, {2, -2}, {-2, 2}, {3, 0}, {-3, 0}, {0, 3}, {0, -3}, {3, 3}, {-3, -3}, {3, -3}, {-3, 3}, {4, 0}, {-4, 0}, {0, 4}, {0, -4}, {4, 4}, {-4, -4}, {4, -4}, {-4, 4}, {5, 0}, {-5, 0}, {0, 5}, {0, -5}, {5, 5}, {-5, -5}, {5, -5}, {-5, 5}, {6, 0}, {-6, 0}, {0, 6}, {0, -6}, {6, 6}, {-6, -6}, {6, -6}, {-6, 6}, {7, 0}, {-7, 0}, {0, 7}, {0, -7}, {7, 7}, {-7, -7}, {7, -7}, {-7, 7}};
        case Piece::White_Rook: case Piece::Black_Rook: return {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {2, 0}, {-2, 0}, {0, 2}, {0, -2}, {3, 0}, {-3, 0}, {0, 3}, {0, -3}, {4, 0}, {-4, 0}, {0, 4}, {0, -4}, {5, 0}, {-5, 0}, {0, 5}, {0, -5}, {6, 0}, {-6, 0}, {0, 6}, {0, -6}, {7, 0}, {-7, 0}, {0, 7}, {0, -7}};
        case Piece::White_Bishop: case Piece::Black_Bishop: return {{1, 1}, {-1, -1}, {1, -1}, {-1, 1}, {2, 2}, {-2, -2}, {2, -2}, {-2, 2}, {3, 3}, {-3, -3}, {3, -3}, {-3, 3}, {4, 4}, {-4, -4}, {4, -4}, {-4, 4}, {5, 5}, {-5, -5}, {5, -5}, {-5, 5}, {6, 6}, {-6, -6}, {6, -6}, {-6, 6}, {7, 7}, {-7, -7}, {7, -7}, {-7, 7}};
        case Piece::White_Knight: case Piece::Black_Knight: return {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {-1, 2}, {1, -2}, {-1, -2}};
        case Piece::White_Pawn: case Piece::Black_Pawn: return {{1, 0}}; // Does not include two moves forward, will be include if white is on the 2nd rank, or if black is on the 7th.
        case Piece::Empty: default: return {};
    }
}

char piece_to_string(Piece p) {
    switch (p) {
        case Piece::White_King: return 'K';
        case Piece::White_Queen: return 'Q';
        case Piece::White_Bishop: return 'B';
        case Piece::White_Knight: return 'N';
        case Piece::White_Rook: return 'R';
        case Piece::White_Pawn: return 'P';
        case Piece::Black_King: return 'k';
        case Piece::Black_Queen: return 'q';
        case Piece::Black_Bishop: return 'b';
        case Piece::Black_Knight: return 'n';
        case Piece::Black_Rook: return 'r';
        case Piece::Black_Pawn: return 'p';
        default: return ' ';
    }
}

Piece string_to_piece(char c) {
    switch (c) {
        case 'K': return Piece::White_King;
        case 'Q': return Piece::White_Queen;
        case 'B': return Piece::White_Bishop;
        case 'N': return Piece::White_Knight;
        case 'R': return Piece::White_Rook;
        case 'P': return Piece::White_Pawn;
        case 'k': return Piece::Black_King;
        case 'q': return Piece::Black_Queen;
        case 'b': return Piece::Black_Bishop;
        case 'n': return Piece::Black_Knight;
        case 'r': return Piece::Black_Rook;
        case 'p': return Piece::Black_Pawn;
        default: return Piece::Empty;
    }
}

void Board::children() { // TODO: Test
    std::vector<Move> pseudo;
    pseudo.reserve(218); // https://www.chessprogramming.org/Chess_Position

    auto in_bounds = [](int r, int f) {
        return r >= 0 && r < 8 && f >= 0 && f < 8;
    };

    auto opponent = [&](Side s) {
        if (s == Side::White) return Side::Black;
        if (s == Side::Black) return Side::White;
        return None;
    };

    auto is_enemy = [&](Piece p) {
        Side s = Game::side_of_piece(p);
        return s != None && s != toMove;
    };

    auto add_if_ok = [&](int fr, int ff, int tr, int tf) {
        if (!in_bounds(tr, tf)) return;
        Piece target = board[tr * 8 + tf];
        if (Game::side_of_piece(target) == toMove) return;
        pseudo.emplace_back(fr, ff, tr, tf);
    };

    auto pin_ray = [&](int fr, int ff, int dr, int df) {
        for (int step = 1; step < 8; ++step) {
            int tr = fr + dr * step;
            int tf = ff + df * step;
            if (!in_bounds(tr, tf)) break;

            Piece target = board[tr * 8 + tf];
            if (target == Empty) {
                pseudo.emplace_back(fr, ff, tr, tf);
                continue;
            }

            if (Game::side_of_piece(target) != toMove) {
                pseudo.emplace_back(fr, ff, tr, tf);
            }
            break;
        }
    };

    auto attacked_by_board = [&](const std::array<Piece, 64>& b, Side attacker, int sr, int sf) {
        auto inb = [&](int r, int f) { return r >= 0 && r < 8 && f >= 0 && f < 8; };

        // Pawn attacks
        if (attacker == White) {
            int pr = sr + 1;
            if (pr < 8) {
                if (sf - 1 >= 0 && b[pr * 8 + sf - 1] == White_Pawn) return true;
                if (sf + 1 < 8 && b[pr * 8 + sf + 1] == White_Pawn) return true;
            }
        } else if (attacker == Black) {
            int pr = sr - 1;
            if (pr >= 0) {
                if (sf - 1 >= 0 && b[pr * 8 + sf - 1] == Black_Pawn) return true;
                if (sf + 1 < 8 && b[pr * 8 + sf + 1] == Black_Pawn) return true;
            }
        }

        // Knight attacks
        static const int kdr[8] = {2, 2, -2, -2, 1, 1, -1, -1};
        static const int kdf[8] = {1, -1, 1, -1, 2, -2, 2, -2};
        for (int i = 0; i < 8; ++i) {
            int r = sr + kdr[i];
            int f = sf + kdf[i];
            if (!inb(r, f)) continue;
            Piece p = b[r * 8 + f];
            if (attacker == White && p == White_Knight) return true;
            if (attacker == Black && p == Black_Knight) return true;
        }

        // King attacks
        for (int dr = -1; dr <= 1; ++dr) {
            for (int df = -1; df <= 1; ++df) {
                if (dr == 0 && df == 0) continue;
                int r = sr + dr;
                int f = sf + df;
                if (!inb(r, f)) continue;
                Piece p = board[r * 8 + f];
                if (attacker == White && p == White_King) return true;
                if (attacker == Black && p == Black_King) return true;
            }
        }

        // Sliding attacks
        auto ray_hits = [&](int dr, int df, bool rook_like, bool bishop_like) {
            for (int step = 1; step < 8; ++step) {
                int r = sr + dr * step;
                int f = sf + df * step;
                if (!inb(r, f)) return false;
                Piece p = b[r * 8 + f];
                if (p == Empty) continue;
                if (attacker == White) {
                    if (rook_like && (p == White_Rook || p == White_Queen)) return true;
                    if (bishop_like && (p == White_Bishop || p == White_Queen)) return true;
                } else {
                    if (rook_like && (p == Black_Rook || p == Black_Queen)) return true;
                    if (bishop_like && (p == Black_Bishop || p == Black_Queen)) return true;
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

    auto find_king = [&]( Side s, int& kr, int& kf) {
        Piece king = (s == White) ? White_King : Black_King;
        for (int r = 0; r < 8; ++r)
            for (int f = 0; f < 8; ++f)
                if (board[r * 8 + f] == king) { kr = r; kf = f; return true; }
        return false;
    };

    auto apply_move_on = [&](std::array<Piece, 64>& b, const Move& m) {
        Piece moving   = b[m.from.rank * 8 + m.from.file];
        Piece captured = b[m.to.rank * 8 + m.to.file];

        b[m.to.rank * 8 + m.to.file] = moving;
        b[m.from.rank * 8 + m.from.file] = Empty;

        // en-passant capture (use st.enPassant, not Game::en_passant)
        if ((moving == White_Pawn || moving == Black_Pawn) &&
                captured == Empty &&
                enPassant.is_valid() &&
                m.to == enPassant &&
                std::abs(m.to.file - m.from.file) == 1) {
            int cap_rank = (moving == White_Pawn) ? (m.to.rank + 1) : (m.to.rank - 1);
            if (cap_rank >= 0 && cap_rank < 8)
                b[cap_rank * 8 + m.to.file] = Empty;
        }

        // castling rook move
        if ((moving == White_King || moving == Black_King) &&
                m.from.rank == m.to.rank &&
                std::abs(m.to.file - m.from.file) == 2) {
            int rank = m.from.rank;
            if (m.to.file == 6) { // king-side
                b[rank * 8 + 5] = b[rank * 8 + 7];
                b[rank * 8 + 7] = Empty;
            } else if (m.to.file == 2) { // queen-side
                b[rank * 8 + 3] = b[rank * 8 + 0];
                b[rank * 8 + 0] = Empty;
            }
        }

        // promotion
        if (moving == White_Pawn && m.to.rank == 0) {
            Piece promo = (m.promotion == Empty) ? White_Queen : m.promotion;
            if (Game::side_of_piece(promo) != White) promo = White_Queen;
            b[m.to.rank * 8 + m.to.file] = promo;
        } else if (moving == Black_Pawn && m.to.rank == 7) {
            Piece promo = (m.promotion == Empty) ? Black_Queen : m.promotion;
            if (Game::side_of_piece(promo) != Black) promo = Black_Queen;
            b[m.to.rank * 8 + m.to.file] = promo;
        }
    };

    for (int r = 0; r < 8; ++r) {
        for (int f = 0; f < 8; ++f) {
            Piece p = board[r * 8 + f];
            if (Game::side_of_piece(p) != toMove) continue;

            if (p == White_Pawn || p == Black_Pawn) {
                int dir        = (p == White_Pawn) ? -1 : 1;
                int start_rank = (p == White_Pawn) ? 6 : 1;
                int promo_rank = (p == White_Pawn) ? 0 : 7;

                auto add_pawn_move = [&](int fr, int ff, int tr, int tf) {
                    if (tr == promo_rank) {
                        if (toMove == White) {
                            pseudo.emplace_back(fr, ff, tr, tf, White_Queen);
                            pseudo.emplace_back(fr, ff, tr, tf, White_Rook);
                            pseudo.emplace_back(fr, ff, tr, tf, White_Bishop);
                            pseudo.emplace_back(fr, ff, tr, tf, White_Knight);
                        } else {
                            pseudo.emplace_back(fr, ff, tr, tf, Black_Queen);
                            pseudo.emplace_back(fr, ff, tr, tf, Black_Rook);
                            pseudo.emplace_back(fr, ff, tr, tf, Black_Bishop);
                            pseudo.emplace_back(fr, ff, tr, tf, Black_Knight);
                        }
                    } else {
                        pseudo.emplace_back(fr, ff, tr, tf);
                    }
                };

                int r1 = r + dir;
                if (in_bounds(r1, f) && board[r1 * 8 + f] == Empty) {
                    add_pawn_move(r, f, r1, f);

                    int r2 = r + 2 * dir;
                    if (r == start_rank && in_bounds(r2, f) && board[r2 * 8 + f] == Empty) {
                        pseudo.emplace_back(r, f, r2, f);
                    }
                }

                for (int df : {-1, 1}) {
                    int tf = f + df;
                    int tr = r + dir;
                    if (!in_bounds(tr, tf)) continue;

                    Piece target = board[tr * 8 + tf];
                    if (is_enemy(target)) add_pawn_move(r, f, tr, tf);

                    if (enPassant.rank != -1 && enPassant.file != -1 && enPassant.rank == tr && enPassant.file == tf) {
                        pseudo.emplace_back(r, f, tr, tf);
                    }
                }

                continue;
            }

            if (p == White_Rook || p == Black_Rook) {
                pin_ray(r, f, 1, 0); pin_ray(r, f, -1, 0);
                pin_ray(r, f, 0, 1); pin_ray(r, f, 0, -1);
                continue;
            }
            if (p == White_Bishop || p == Black_Bishop) {
                pin_ray(r, f, 1, 1); pin_ray(r, f, 1, -1);
                pin_ray(r, f, -1, 1); pin_ray(r, f, -1, -1);
                continue;
            }
            if (p == White_Queen || p == Black_Queen) {
                pin_ray(r, f, 1, 0); pin_ray(r, f, -1, 0);
                pin_ray(r, f, 0, 1); pin_ray(r, f, 0, -1);
                pin_ray(r, f, 1, 1); pin_ray(r, f, 1, -1);
                pin_ray(r, f, -1, 1); pin_ray(r, f, -1, -1);
                continue;
            }

            if (p == White_King || p == Black_King) {
                for (auto [dr, df] : get_piece_moves(p))
                    add_if_ok(r, f, r + dr, f + df);

                Side opp = opponent(toMove);

                if (toMove == White && r == 7 && f == 4) {
                    if (whiteKingSide &&
                            board[7 * 8 + 7] == White_Rook &&
                            board[7 * 8 + 5] == Empty && board[7 * 8 + 6] == Empty &&
                            !attacked_by_board(board, opp, 7, 4) &&
                            !attacked_by_board(board, opp, 7, 5) &&
                            !attacked_by_board(board, opp, 7, 6)) {
                        pseudo.emplace_back(7, 4, 7, 6);
                    }
                    if (whiteQueenSide &&
                            board[7 * 8 + 0] == White_Rook &&
                            board[7 * 8 + 1] == Empty && board[7 * 8 + 2] == Empty && board[7 * 8 + 3] == Empty &&
                            !attacked_by_board(board, opp, 7, 4) &&
                            !attacked_by_board(board, opp, 7, 3) &&
                            !attacked_by_board(board, opp, 7, 2)) {
                        pseudo.emplace_back(7, 4, 7, 2);
                    }
                }

                if (toMove == Black && r == 0 && f == 4) {
                    if (blackKingSide &&
                            board[0 * 8 + 7] == Black_Rook &&
                            board[0 * 8 + 5] == Empty && board[0 * 8 + 6] == Empty &&
                            !attacked_by_board(board, opp, 0, 4) &&
                            !attacked_by_board(board, opp, 0, 5) &&
                            !attacked_by_board(board, opp, 0, 6)) {
                        pseudo.emplace_back(0, 4, 0, 6);
                    }
                    if (blackQueenSide &&
                            board[0 * 8 + 0] == Black_Rook &&
                            board[0 * 8 + 1] == Empty && board[0 * 8 + 2] == Empty && board[0 * 8 + 3] == Empty &&
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

    Side opp = opponent(toMove);

    int kr = -1, kf = -1;
    if (!find_king(toMove, kr, kf)) {
        moves = pseudo;
        return;
    }

    moves.clear();

    moves.reserve(pseudo.size());
    
    for (const Move& m : pseudo) {
        std::array<Piece, 64> b = board;
        apply_move_on(b, m);

        int tkr = kr;
        int tkf = kf;

        Piece moving = board[m.from.rank * 8 + m.from.file];
        if (moving == Piece::White_King || moving == Piece::Black_King) {
            tkr = m.to.rank;
            tkf = m.to.file;
        }

        if (attacked_by_board(b, opp, tkr, tkf))
            continue;

        moves.push_back(m);
    }
}

std::optional<Board> Board::update(const Board& b, const Move& move) { // TODO: Test
    for (const Move& m : b.moves) {
        bool same_squares = (move.from == m.from && move.to == m.to);
        bool promo_ok =
            (move.promotion == m.promotion) ||
            (move.promotion == Empty && m.promotion != Empty);

        if (!same_squares || !promo_ok) {
            continue;
        }

        Board board = b;

        Move chosen = m;
        if (move.promotion != Empty) {
            chosen.promotion = move.promotion;
        }

        Piece moving = board.board[chosen.from.rank * 8 + chosen.from.file];
        Piece captured = board.board[chosen.to.rank * 8 + chosen.to.file];

        Square old_ep = board.enPassant;

        if (moving == White_Pawn || moving == Black_Pawn || captured != Empty) {
            board.halfMove = 0;
        } else {
            ++board.halfMove;
        }

        board.board[chosen.to.rank * 8 + chosen.to.file] = moving;
        board.board[chosen.from.rank * 8 + chosen.from.file] = Empty;

        // en passant capture
        if ((moving == White_Pawn || moving == Black_Pawn) &&
                captured == Empty &&
                old_ep.rank != -1 && old_ep.file != -1 &&
                chosen.to == old_ep &&
                std::abs(chosen.to.file - chosen.from.file) == 1) {
            int cap_rank = (moving == White_Pawn)
                ? (chosen.to.rank + 1)
                : (chosen.to.rank - 1);

            if (cap_rank >= 0 && cap_rank < 8) {
                board.board[cap_rank * 8 + chosen.to.file] = Empty;
            }
        }

        board.enPassant = Square();

        if (moving == White_Pawn && chosen.from.rank == 6 && chosen.to.rank == 4) {
            board.enPassant = Square(5, chosen.from.file);
        } else if (moving == Black_Pawn && chosen.from.rank == 1 && chosen.to.rank == 3) {
            board.enPassant = Square(2, chosen.from.file);
        }

        // castling rook move
        if ((moving == White_King || moving == Black_King) &&
                chosen.from.rank == chosen.to.rank &&
                std::abs(chosen.to.file - chosen.from.file) == 2) {
            int rank = chosen.from.rank;
            if (chosen.to.file == 6) {
                Piece rook = board.board[rank * 8 + 7];
                board.board[rank * 8 + 5] = rook;
                board.board[rank * 8 + 7] = Piece::Empty;
            } else if (chosen.to.file == 2) {
                Piece rook = board.board[rank * 8 + 0];
                board.board[rank * 8 + 3] = rook;
                board.board[rank * 8 + 0] = Piece::Empty;
            }
        }

        // promotion
        if (moving == White_Pawn && chosen.to.rank == 0) {
            Piece promo = chosen.promotion;
            if (promo == Empty) promo = White_Queen;
            if (Game::side_of_piece(promo) != White) promo = White_Queen;
            board.board[chosen.to.rank * 8 + chosen.to.file] = promo;
        } else if (moving == Black_Pawn && chosen.to.rank == 7) {
            Piece promo = chosen.promotion;
            if (promo == Empty) promo = Black_Queen;
            if (Game::side_of_piece(promo) != Black) promo = Black_Queen;
            board.board[chosen.to.rank * 8 + chosen.to.file] = promo;
        }

        // castling rights
        if (moving == White_King) {
            board.whiteKingSide = false;
            board.whiteQueenSide = false;
        } else if (moving == Black_King) {
            board.blackKingSide = false;
            board.blackQueenSide = false;
        } else if (moving == White_Rook) {
            if (chosen.from.rank == 7 && chosen.from.file == 0) board.whiteQueenSide = false;
            if (chosen.from.rank == 7 && chosen.from.file == 7) board.whiteKingSide = false;
        } else if (moving == Black_Rook) {
            if (chosen.from.rank == 0 && chosen.from.file == 0) board.blackQueenSide = false;
            if (chosen.from.rank == 0 && chosen.from.file == 7) board.blackKingSide = false;
        }

        if (captured == White_Rook) {
            if (chosen.to.rank == 7 && chosen.to.file == 0) board.whiteQueenSide = false;
            if (chosen.to.rank == 7 && chosen.to.file == 7) board.whiteKingSide = false;
        } else if (captured == Black_Rook) {
            if (chosen.to.rank == 0 && chosen.to.file == 0) board.blackQueenSide = false;
            if (chosen.to.rank == 0 && chosen.to.file == 7) board.blackKingSide = false;
        }

        if (board.toMove == Side::Black) {
            ++board.fullMove;
        }
        board.toMove = (board.toMove == Side::White) ? Side::Black : Side::White;

        return std::make_optional(board);
    }

    return std::nullopt;
}

double Board::evaluate() const {
    constexpr std::array<double, 64> evalKing = {{
        -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -2, -2, -1, -1, -1,
            -1, -1, -1, -2, -2, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1,
            0,  0,  2, -1,  1, -1,  2,  0
    }};

    constexpr std::array<double, 64> evalQueen = {{
        2, 2, 2, 2, 2, 2, 2, 2,
            2, 2, 2, 2, 2, 2, 2, 2,
            2, 2, 2, 2, 2, 2, 2, 2,
            2, 2, 2, 2, 2, 2, 2, 2,
            2, 2, 2, 2, 2, 2, 2, 2,
            2, 2, 2, 2, 2, 2, 2, 2,
            2, 2, 2, 2, 2, 2, 2, 2,
            2, 2, 2, 2, 2, 2, 2, 2
    }};

    constexpr std::array<double, 64> evalBishop = {{
        2, 2, 2, 2, 2, 2, 2, 2,
            2, 1, 1, 1, 1, 1, 1, 2,
            2, 1, 0, 0, 0, 0, 1, 2,
            2, 1, 0, .5, .5, 0, 1, 2,
            2, 1, 0, .5, .5, 0, 1, 2,
            2, 1, 0, 0, 0, 0, 1, 2,
            2, 1, 1, 1, 1, 1, 1, 2,
            2, 2, 2, 2, 2, 2, 2, 2
    }};

    constexpr std::array<double, 64> evalKnight = {{
        0, 0, 0, 0, 0, 0, 0, 0,
            0, .5, .5, .5, .5, .5, .5, 0,
            0, .5, 1, 1, 1, 1, .5, 0,
            0, .5, 1, 1, 1, 1, .5, 0,
            0, .5, 1, 1, 1, 1, .5, 0,
            0, .5, 1, 1, 1, 1, .5, 0,
            0, .5, .5, .5, .5, .5, .5, 0,
            0, .5, 0, 0, 0, 0, .5, 0
    }};

    constexpr std::array<double, 64> evalRook = {{
        0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,
            -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1,
            0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  2,  0,  1,  0,  2,  0
    }};

    constexpr std::array<double, 64> evalPawn = {{
        5,    5,    5,    5,    5,    5,    5,    5,
            2.5,  2.75, 2.75, 2.75, 2.75, 2.75, 2.75, 2.5,
            2.5,  2.5,  2.5,  2.5,  2.5,  2.5,  2.5,  2.5,
            2,    2.25, 2.25, 2.25, 2.25, 2.25, 2.25, 2,
            1,    1,    2,    4,    4,    2,    1,    1,
            1,    1.125,1.25, 1.25, 1.25, 1.25, 1.125,1,
            1,    1,    1,    1,    1,    1,    1,    1,
            0,    0,    0,    0,    0,    0,    0,    0
    }};

    double whiteScore = 0.0;
    double blackScore = 0.0;

    for (int i = 0; i < 64; ++i) {
        const int mirrored = 63 - i;

        switch (board[i]) {
            case Piece::White_King:
                whiteScore += evalKing[i];
                break;
            case Piece::White_Queen:
                whiteScore += evalQueen[i] + 8;
                break;
            case Piece::White_Bishop:
                whiteScore += evalBishop[i] + 3;
                break;
            case Piece::White_Knight:
                whiteScore += evalKnight[i] + 3;
                break;
            case Piece::White_Rook:
                whiteScore += evalRook[i] + 5;
                break;
            case Piece::White_Pawn:
                whiteScore += evalPawn[i] + 1;
                break;

            case Piece::Black_King:
                blackScore += evalKing[mirrored];
                break;
            case Piece::Black_Queen:
                blackScore += evalQueen[mirrored] + 8;
                break;
            case Piece::Black_Bishop:
                blackScore += evalBishop[mirrored] + 3;
                break;
            case Piece::Black_Knight:
                blackScore += evalKnight[mirrored] + 3;
                break;
            case Piece::Black_Rook:
                blackScore += evalRook[mirrored] + 5;
                break;
            case Piece::Black_Pawn:
                blackScore += evalPawn[mirrored] + 1;
                break;

            default:
                break;
        }
    }

    return whiteScore - blackScore;
}

Side Board::checkmate() {
    if (moves.empty())
        return (toMove == White) ? Black : White;
    if (halfMove > 100)
        return Draw;
    return None;
}

void Board::check_moves() {
    children();
    evaluate();
}

std::string Board::get_board_state() {
    std::string notation = "";

    int count = 0;
    for (int i = 63; i >= 0; i--) {
        char m = piece_to_string(board[i]);

        if (m == ' ') {
            count++;
        } else {
            if (count > 0) {
                notation += std::to_string(count);
                count = 0;
            }

            notation += m;
        }

        if (i % 8 == 0) { // End of a row
            if (count > 0) {
                notation += std::to_string(count);
                count = 0;
            }

            if (i != 0)
                notation += "/";
        }
    }

    notation += (toMove == White) ? " w " : " b ";

    notation += (whiteKingSide) ? "K" : "";
    notation += (whiteQueenSide) ? "Q" : "";
    notation += (blackKingSide) ? "k" : "";
    notation += (blackQueenSide) ? "q" : "";

    notation += (!(whiteKingSide || whiteQueenSide || blackKingSide || blackQueenSide)) ? "- " : " ";

    notation += enPassant.to_string() + " ";

    notation += std::to_string(halfMove) + " " + std::to_string(fullMove);

    return notation;
}

void Board::give_board_state(std::string notation) {
    toMove = White;
    whiteKingSide = whiteQueenSide = blackKingSide = blackQueenSide = false;
    enPassant = Square();
    halfMove = 0;
    fullMove = 1;
    board = std::array<Piece, 64>();

    size_t i = 0;

    {
        int rank = 0, file = 0;
        for (; i < notation.length(); i++) {
            if (notation[i] == ' ')
                break;

            if (notation[i] == '/') {
                rank += 1;
                file = 0;
                continue;
            }

            if (std::isdigit(notation[i])) {
                file += notation[i] - '0';
                continue;
            }

            board[63 - (rank * 8 + file)] = string_to_piece(notation[i]);
            file++;
        }
    }

    if ( notation[++i] == 'b' )
        toMove = Black;

    i += 2;
    if ( notation[i] != '-' ) {
        if ( notation[i] == 'K' ) {
            whiteKingSide = true;
            ++i;
        }
        if ( notation[i] == 'Q' ) {
            whiteQueenSide = true;
            ++i;
        }
        if ( notation[i] == 'k' ) {
            blackKingSide = true;
            ++i;
        }
        if ( notation[i] == 'q' ) {
            blackQueenSide = true;
            ++i;
        }
    }

    i += 1;
    if ( notation[i] != '-' ) {
        enPassant = Square(notation[i], notation[i + 1]);
        ++i;
    }

    i += 2;
    int j = 0;
    for (; std::isdigit(notation[i + j]); j++) { } // Find the length of the half move clock
    halfMove = std::stoi(notation.substr(i, j));
    i += j + 1;

    j = 0;
    for (; std::isdigit(notation[i + j]); j++) { } // Find the length of the full move clock
    fullMove = std::stoi(notation.substr(i, j));
}

std::string Board::print_moves() { // TODO: test
    std::string ret = get_board_state()
        + "\nMoves: " + std::to_string(moves.size())
        + "\t To Move: " + ((toMove == White) ? "White" : "Black") + "\n";

    for (Move m : moves)
        ret += m.to_string() + "\n";

    return ret;
}

std::string Board::print_board() {
    std::string ret;

    ret += "  a b c d e f g h\n\n";

    for (int r = 7; r >= 0; r--) {

        std::string top;
        std::string bottom;

        top += std::to_string(8 - r);
        top += ' ';
        bottom += "  ";

        for (int f = 7; f >= 0; f--) {

            bool whiteSquare = ((r + f) % 2 == 0);

            const char* bg = whiteSquare ? WHITE_BG : BLACK_BG;
            const char* fg = whiteSquare ? BLACK_TEXT : WHITE_TEXT;

            char p = piece_to_string(board[r * 8 + f]);

            top += bg;
            top += fg;
            top += p;
            top += " ";
            top += RESET;

            bottom += bg;
            bottom += "  ";
            bottom += RESET;
        }

        ret += top + "\n";
        ret += bottom + "\n";
    }

    return ret;
}

Game::Game(std::string notation) {
    give_board_state(notation);
}

void Game::check_moves() {
    return curr.check_moves();
}

Side Game::side_of_piece(Piece p) {
    return static_cast<Side>((int)(p / 10));
}

double Game::alphabeta(const Board& node, int depth, double alpha, double beta, bool maxPlayer, std::vector<Move>& pv) { // TODO: Test
    if (depth == 0 || node.moves.empty()) {
        pv.clear();
        return node.evaluate();
    }

    double bestScore = maxPlayer ? MIN_SCORE : MAX_SCORE;
    std::vector<Move> bestLine;

    for (const Move& mx : node.moves) {
        auto next = Board::update(node, mx);
        if (!next)
            continue;

        Board b = *next;
        std::vector<Move> childLine;

        double score = alphabeta(b, depth - 1, alpha, beta, !maxPlayer, childLine);

        bool better = maxPlayer ? (score > bestScore) : (score < bestScore);
        if (better) {
            bestScore = score;
            bestLine.clear();
            bestLine.push_back(mx);
            bestLine.insert(bestLine.end(), childLine.begin(), childLine.end());
        }

        if (maxPlayer) {
            alpha = std::max(alpha, bestScore);
            if (alpha >= beta) {
                break;
            }
        } else {
            beta = std::min(beta, bestScore);
            if (beta <= alpha) {
                break;
            }
        }
    }

    pv = std::move(bestLine);
    return bestScore;
}

std::string Game::get_board_state() {
    return curr.get_board_state();
}

void Game::give_board_state(std::string state) {
    curr.give_board_state(state);
}

Move Game::get_move() { // TODO: Update to be multi-threaded
    if (curr.checkmate() != None)
        return Move();

    bestMoves.clear();
    alphabeta(curr, SEARCH_DEPTH, MIN_SCORE, MAX_SCORE, true, bestMoves);

    return bestMoves.empty() ? Move{} : bestMoves.front();
}

bool Game::give_move(Move move) {
    if (checkmate() != None)
        return false;

    curr = *Board::update(curr, move);
    completed.push_back(move);
    check_moves();
    return true;
}

bool Game::give_move(Square from, Square to, Piece promo) {
    return give_move(Move(from, to, promo));
}

std::string Game::end_game() {
    std::string ret;

    Side winner = checkmate();
    switch (winner) {
        case White:
            ret += "White Wins!\n";
            break;
        case Black:
            ret += "Black Wins!\n";
            break;
        case Draw:
            ret += "Game Drawn!\n";
            break;
        default:
            ret += "No Winner.\n";
            break;
    }

    ret += print_moves() + "\n";

    return ret;
}

std::string Game::print_moves() {
    return curr.print_moves();
}

std::string Game::print_board() {
    return curr.print_board();
}

double Game::print_score() {
    return curr.score;
}

Side Game::checkmate() {
    return curr.checkmate();
}
