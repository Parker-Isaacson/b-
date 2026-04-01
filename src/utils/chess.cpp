#include "chess.h"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <string>

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

Side Game::side_of_piece(Piece p) {
    switch (p) {
        case Piece::White_King: case Piece::White_Queen: case Piece::White_Bishop: case Piece::White_Knight: case Piece::White_Rook: case Piece::White_Pawn: return Side::White;
        case Piece::Black_King: case Piece::Black_Queen: case Piece::Black_Bishop: case Piece::Black_Knight: case Piece::Black_Rook: case Piece::Black_Pawn: return Side::Black;
        default: return Side::Empty;
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

    state = PositionState(Side::White, Square(), CastlingRights(true, true, true, true));

    check_moves();
}

Game::Game(std::string notation) {
    give_board_state(notation);
    checkmate();
}

std::string Game::get_board_state() {
    std::string notation = "";

    int rankCount = 0;
    for (std::array<Piece, 8> rank : board) {
        int count = 0;
        for (Piece p : rank) {
            char m = piece_to_string(p);
            if ( m == ' ' ) {
                count++;
                continue;
            }

            if ( count > 0 ) {
                notation += std::to_string(count);
                count = 0;
            }
            
            notation += m;
        }
        
        if ( count > 0 )
            notation += std::to_string(count);
        
        rankCount++;

        if ( rankCount != 8 )
            notation += "/";
    }

    notation += ( state.toMove == Side::White) ? " w " : " b ";

    notation += ( state.castle.whiteKingSide ) ? "K" : "";
    notation += ( state.castle.whiteQueenSide ) ? "Q" : "";
    notation += ( state.castle.blackKingSide ) ? "k" : "";
    notation += ( state.castle.blackQueenSide ) ? "q" : "";

    notation += ( ! (state.castle.whiteKingSide || state.castle.whiteQueenSide || state.castle.blackKingSide || state.castle.blackQueenSide) ) ? "- " : " ";

    notation += state.enPassant.to_string() + " ";

    notation += std::to_string(state.halfMove) + " " + std::to_string(state.fullMove);

    return notation;
}

void Game::give_board_state(std::string notation) {
    state.toMove = Side::White;

    state.castle.whiteKingSide = state.castle.whiteQueenSide = state.castle.blackKingSide = state.castle.blackQueenSide = false;

    state.enPassant = Square();

    state.halfMove = 0;
    state.fullMove = 1;

    board = Board{};
    
    size_t i = 0;

    { // To scope rank and file
        int rank = 0, file = 0;
        for (; i < notation.length(); i++) {
            if ( notation[i] == ' ' )
                break;

            if ( notation[i] ==  '/' ) {
                rank += 1;
                file = 0;
                continue;
            }

            if (std::isdigit(notation[i])) {
                int n = notation[i] - '0'; // char "cast" to int
                for (int j = 0; j < n; j++) {
                    board[rank][file] = Piece::Empty;
                    file++;
                }
                continue;
            }

            board[rank][file] = string_to_piece(notation[i]);
            file++;
        }
    }

    if ( notation[++i] == 'b' )
        state.toMove = Side::Black;

    i += 2;
    if ( notation[i] != '-' ) {
        if ( notation[i] == 'K' ) {
            state.castle.whiteKingSide = true;
            ++i;
        }
        if ( notation[i] == 'Q' ) {
            state.castle.whiteQueenSide = true;
            ++i;
        }
        if ( notation[i] == 'k' ) {
            state.castle.blackKingSide = true;
            ++i;
        }
        if ( notation[i] == 'q' ) {
            state.castle.blackQueenSide = true;
            ++i;
        }
    }

    i += 1;
    if ( notation[i] != '-' ) {
        state.enPassant = Square(notation[i], notation[i + 1]);
        ++i;
    }

    i += 2;
    int j = 0;
    for (; std::isdigit(notation[i + j]); j++) { } // Find the length of the half move clock
    state.halfMove = std::stoi(notation.substr(i, j));
    i += j + 1;

    j = 0;
    for (; std::isdigit(notation[i + j]); j++) { } // Find the length of the full move clock
    state.fullMove = std::stoi(notation.substr(i, j));

    completed.clear();
    check_moves();
}

double Game::evaluate(const Board& b, const PositionState& /*state*/) {
    double whiteScore = 0;
    double blackScore = 0;

    for (int r = 0; r < 8; r++) {
        for (int f = 0; f < 8; f++) {
            switch (b[r][f]) {
                case Piece::White_King:
                    whiteScore += evalKing[r][f];
                    break;
                case Piece::White_Queen:
                    whiteScore += evalQueen[r][f] + 8;
                    break;
                case Piece::White_Bishop:
                    whiteScore += evalBishop[r][f] + 3;
                    break;
                case Piece::White_Knight:
                    whiteScore += evalKnight[r][f] + 3;
                    break;
                case Piece::White_Rook:
                    whiteScore += evalRook[r][f] + 5;
                    break;
                case Piece::White_Pawn:
                    whiteScore += evalPawn[r][f] + 1;
                    break;

                case Piece::Black_King:
                    blackScore += evalKing[7 - r][7 - f];
                    break;
                case Piece::Black_Queen:
                    blackScore += evalQueen[7 - r][7 - f] + 8;
                    break;
                case Piece::Black_Bishop:
                    blackScore += evalBishop[7 - r][7 - f] + 3;
                    break;
                case Piece::Black_Knight:
                    blackScore += evalKnight[7 - r][7 - f] + 3;
                    break;
                case Piece::Black_Rook:
                    blackScore += evalRook[7 - r][7 - f] + 5;
                    break;
                case Piece::Black_Pawn:
                    blackScore += evalPawn[7 - r][7 - f] + 1;
                    break;

                default:
                    break;
            }
        }
    }

    // Always score from White's perspective.
    return whiteScore - blackScore;
}

// https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning#Pseudocode
Move Game::get_move() {
    auto alphabeta_runner = [&](auto&& self, Board node, PositionState state, int depth, double alpha, double beta, bool maxPlayer, std::vector<Move>& pv) -> double {
        std::vector<Move> child = children(node, state);

        if (depth == 0 || child.empty()) {
            pv.clear();
            return evaluate(node, state);
        }

        double bestScore = maxPlayer ? MIN_SCORE : MAX_SCORE;
        std::vector<Move> bestLine;

        for (Move mx : child) {
            auto next = update_board(node, state, mx, child);
            if (!next) {
                continue;
            }

            auto [b, s] = *next;

            std::vector<Move> childLine;

            double score = self(self, b, s, depth - 1, alpha, beta, !maxPlayer, childLine);

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
    };

    if (checkmate() != Side::Empty) {
        return Move();
    }

    bestMoves.clear();

    const bool maxPlayer = (state.toMove == Side::White);
    alphabeta_runner(alphabeta_runner, board, state, SEARCH_DEPTH, MIN_SCORE, MAX_SCORE, maxPlayer, bestMoves);

    return bestMoves.empty() ? Move{} : bestMoves.front();
}

bool Game::give_move(Move move) {
    if (checkmate() != Side::Empty)
        return false;

    auto next = update_board(board, state, move, moves);
    if (!next) {
        return false;
    }

    board = next->first;
    state = next->second;
    completed.push_back(move);
    check_moves();
    return true;
}

bool Game::give_move(Square from, Square to, Piece promo) {
    return give_move(Move(from, to, promo));
}

std::optional<std::pair<Board, PositionState>>
Game::update_board(const Board& oldBoard, const PositionState& oldState, const Move& move, const std::vector<Move>& moves) {
    for (const Move& m : moves) {
        bool same_squares = (move.from == m.from && move.to == m.to);
        bool promo_ok =
            (move.promotion == m.promotion) ||
            (move.promotion == Piece::Empty && m.promotion != Piece::Empty);

        if (!same_squares || !promo_ok) {
            continue;
        }

        Board board = oldBoard;
        PositionState state = oldState;

        Move chosen = m;
        if (move.promotion != Piece::Empty) {
            chosen.promotion = move.promotion;
        }

        Piece moving = board[chosen.from.rank][chosen.from.file];
        Piece captured = board[chosen.to.rank][chosen.to.file];

        Square old_ep = state.enPassant;

        if (moving == Piece::White_Pawn || moving == Piece::Black_Pawn || captured != Piece::Empty) {
            state.halfMove = 0;
        } else {
            ++state.halfMove;
        }

        board[chosen.to.rank][chosen.to.file] = moving;
        board[chosen.from.rank][chosen.from.file] = Piece::Empty;

        // en passant capture
        if ((moving == Piece::White_Pawn || moving == Piece::Black_Pawn) &&
            captured == Piece::Empty &&
            old_ep.rank != -1 && old_ep.file != -1 &&
            chosen.to == old_ep &&
            std::abs(chosen.to.file - chosen.from.file) == 1) {
            int cap_rank = (moving == Piece::White_Pawn)
                ? (chosen.to.rank + 1)
                : (chosen.to.rank - 1);

            if (cap_rank >= 0 && cap_rank < 8) {
                board[cap_rank][chosen.to.file] = Piece::Empty;
            }
        }

        state.enPassant = Square();

        if (moving == Piece::White_Pawn && chosen.from.rank == 6 && chosen.to.rank == 4) {
            state.enPassant = Square(5, chosen.from.file);
        } else if (moving == Piece::Black_Pawn && chosen.from.rank == 1 && chosen.to.rank == 3) {
            state.enPassant = Square(2, chosen.from.file);
        }

        // castling rook move
        if ((moving == Piece::White_King || moving == Piece::Black_King) &&
            chosen.from.rank == chosen.to.rank &&
            std::abs(chosen.to.file - chosen.from.file) == 2) {
            int rank = chosen.from.rank;
            if (chosen.to.file == 6) {
                Piece rook = board[rank][7];
                board[rank][5] = rook;
                board[rank][7] = Piece::Empty;
            } else if (chosen.to.file == 2) {
                Piece rook = board[rank][0];
                board[rank][3] = rook;
                board[rank][0] = Piece::Empty;
            }
        }

        // promotion
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

        // castling rights
        if (moving == Piece::White_King) {
            state.castle.whiteKingSide = false;
            state.castle.whiteQueenSide = false;
        } else if (moving == Piece::Black_King) {
            state.castle.blackKingSide = false;
            state.castle.blackQueenSide = false;
        } else if (moving == Piece::White_Rook) {
            if (chosen.from.rank == 7 && chosen.from.file == 0) state.castle.whiteQueenSide = false;
            if (chosen.from.rank == 7 && chosen.from.file == 7) state.castle.whiteKingSide = false;
        } else if (moving == Piece::Black_Rook) {
            if (chosen.from.rank == 0 && chosen.from.file == 0) state.castle.blackQueenSide = false;
            if (chosen.from.rank == 0 && chosen.from.file == 7) state.castle.blackKingSide = false;
        }

        if (captured == Piece::White_Rook) {
            if (chosen.to.rank == 7 && chosen.to.file == 0) state.castle.whiteQueenSide = false;
            if (chosen.to.rank == 7 && chosen.to.file == 7) state.castle.whiteKingSide = false;
        } else if (captured == Piece::Black_Rook) {
            if (chosen.to.rank == 0 && chosen.to.file == 0) state.castle.blackQueenSide = false;
            if (chosen.to.rank == 0 && chosen.to.file == 7) state.castle.blackKingSide = false;
        }

        if (state.toMove == Side::Black) {
            ++state.fullMove;
        }
        state.toMove = (state.toMove == Side::White) ? Side::Black : Side::White;

        return std::make_optional(std::make_pair(board, state));
    }

    return std::nullopt;
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

    moves = children(board, PositionState(state.toMove, state.enPassant, CastlingRights(state.castle.whiteKingSide, state.castle.whiteQueenSide, state.castle.blackKingSide, state.castle.blackQueenSide)));
    return true;
}

std::string Game::print_moves() {
    std::string ret = get_board_state() + "\nMoves: " + std::to_string(moves.size()) + "\t To Move: " + ((state.toMove == Side::White) ? "White" : "Black") + "\n";

    for (Move m : moves)
        ret += m.to_string() + "\n";

    return ret;
}

Side Game::checkmate() {
    check_moves();
    if (moves.empty())
        return (state.toMove == Side::White) ? Side::Black : Side::White;
    if (state.halfMove > 100)
        return Side::Draw;
    return Side::Empty;
}


std::string Game::end_game() {
    std::string ret;

    Side winner = checkmate();
    if (winner == Side::White)
        ret += "White Wins!\n";
    else if (winner == Side::Black)
        ret += "Black Wins!\n";
    else if (winner == Side::Draw)
        ret += "Game Drawn!\n";
    else
        ret += "No Winner.\n";

    for (Move m : completed)
        ret += m.to_string() + ", ";

    ret += "\n" + get_board_state() + "\n";

    return ret;
}

std::string Game::print_board() {
    std::string ret;

    ret += "  a b c d e f g h\n\n";

    for (int r = 0; r < 8; r++) {

        std::string top;
        std::string bottom;

        top += std::to_string(8 - r);
        top += ' ';
        bottom += "  ";

        for (int f = 0; f < 8; f++) {

            bool whiteSquare = ((r + f) % 2 == 0);

            const char* bg = whiteSquare ? WHITE_BG : BLACK_BG;
            const char* fg = whiteSquare ? BLACK_TEXT : WHITE_TEXT;

            char p = piece_to_string(board[r][f]);

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

double Game::print_score() {
    return evaluate(board, state);
}
