import re
from copy import deepcopy

FILES = "abcdefgh"
RANKS = "12345678"

START_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"


def square_to_xy(sq):
    return FILES.index(sq[0]), int(sq[1]) - 1


def xy_to_square(x, y):
    return FILES[x] + str(y + 1)


def piece_color(piece):
    if piece is None:
        return None
    return "w" if piece.isupper() else "b"


def piece_type(piece):
    return piece.upper() if piece else None


def opponent(color):
    return "b" if color == "w" else "w"


class Position:
    def __init__(self):
        self.board = {}
        self.side_to_move = "w"
        self.castling = "KQkq"
        self.en_passant = "-"
        self.halfmove = 0
        self.fullmove = 1

    @staticmethod
    def from_fen(fen):
        pos = Position()
        parts = fen.strip().split()
        if len(parts) != 6:
            raise ValueError(f"Invalid FEN: {fen}")

        board_part, stm, castling, ep, halfmove, fullmove = parts

        rows = board_part.split("/")
        if len(rows) != 8:
            raise ValueError("Invalid FEN board")

        y = 7
        for row in rows:
            x = 0
            for ch in row:
                if ch.isdigit():
                    x += int(ch)
                else:
                    pos.board[xy_to_square(x, y)] = ch
                    x += 1
            y -= 1

        pos.side_to_move = stm
        pos.castling = castling if castling != "-" else ""
        pos.en_passant = ep
        pos.halfmove = int(halfmove)
        pos.fullmove = int(fullmove)
        return pos

    def copy(self):
        p = Position()
        p.board = self.board.copy()
        p.side_to_move = self.side_to_move
        p.castling = self.castling
        p.en_passant = self.en_passant
        p.halfmove = self.halfmove
        p.fullmove = self.fullmove
        return p

    def piece_at(self, sq):
        return self.board.get(sq)

    def remove_piece(self, sq):
        if sq in self.board:
            del self.board[sq]

    def set_piece(self, sq, piece):
        self.board[sq] = piece

    def king_square(self, color):
        target = "K" if color == "w" else "k"
        for sq, piece in self.board.items():
            if piece == target:
                return sq
        return None

    def fen(self):
        rows = []
        for y in range(7, -1, -1):
            empty = 0
            row = []
            for x in range(8):
                sq = xy_to_square(x, y)
                piece = self.board.get(sq)
                if piece is None:
                    empty += 1
                else:
                    if empty:
                        row.append(str(empty))
                        empty = 0
                    row.append(piece)
            if empty:
                row.append(str(empty))
            rows.append("".join(row))
        board_part = "/".join(rows)
        castling = self.castling if self.castling else "-"
        return f"{board_part} {self.side_to_move} {castling} {self.en_passant} {self.halfmove} {self.fullmove}"


def parse_input(text):
    text = text.strip()

    # Repair squares broken across whitespace/newlines:
    # "b\n2" -> "b2"
    text = re.sub(r'([a-hA-H])\s+([1-8])', r'\1\2', text)

    # Split into whitespace/comma separated tokens
    raw_parts = [p for p in re.split(r"[\s,]+", text) if p]

    # Expand tokens like "c2c4" or "c7c8q"
    parts = []
    for tok in raw_parts:
        if re.fullmatch(r"[a-h][1-8][a-h][1-8][qrbnQRBN]?", tok):
            parts.append(tok[0:2])
            parts.append(tok[2:4])
            if len(tok) == 5:
                parts.append(tok[4])
        else:
            parts.append(tok)

    # Detect trailing FEN
    final_fen = None
    if len(parts) >= 6:
        maybe_fen = " ".join(parts[-6:])
        if re.fullmatch(
            r"[rnbqkpRNBQKP1-8/]+ [wb] (?:[KQkq]+|-) (?:[a-h][36]|-) \d+ \d+",
            maybe_fen,
        ):
            final_fen = maybe_fen
            parts = parts[:-6]

    def is_square(tok):
        return re.fullmatch(r"[a-h][1-8]", tok) is not None

    def is_promo(tok):
        return re.fullmatch(r"[qrbnQRBN]", tok) is not None

    moves = []
    i = 0
    while i < len(parts):
        if i + 1 >= len(parts):
            raise ValueError(f"Incomplete move near end of input: {parts[i:]}")

        frm = parts[i]
        to = parts[i + 1]

        if not is_square(frm) or not is_square(to):
            raise ValueError(f"Expected move squares, got: {frm!r} {to!r}")

        i += 2
        promo = None

        if i < len(parts) and is_promo(parts[i]):
            promo = parts[i].lower()
            i += 1

        moves.append((frm, to, promo))

    return moves, final_fen

def on_board(x, y):
    return 0 <= x < 8 and 0 <= y < 8


def is_square_attacked(pos, sq, by_color):
    x, y = square_to_xy(sq)

    # Pawns
    if by_color == "w":
        for dx in (-1, 1):
            xx, yy = x + dx, y - 1
            if on_board(xx, yy):
                p = pos.piece_at(xy_to_square(xx, yy))
                if p == "P":
                    return True
    else:
        for dx in (-1, 1):
            xx, yy = x + dx, y + 1
            if on_board(xx, yy):
                p = pos.piece_at(xy_to_square(xx, yy))
                if p == "p":
                    return True

    # Knights
    knight_deltas = [(1, 2), (2, 1), (2, -1), (1, -2),
                     (-1, -2), (-2, -1), (-2, 1), (-1, 2)]
    for dx, dy in knight_deltas:
        xx, yy = x + dx, y + dy
        if on_board(xx, yy):
            p = pos.piece_at(xy_to_square(xx, yy))
            if p == ("N" if by_color == "w" else "n"):
                return True

    # Bishops / Queens
    for dx, dy in [(1, 1), (1, -1), (-1, 1), (-1, -1)]:
        xx, yy = x + dx, y + dy
        while on_board(xx, yy):
            p = pos.piece_at(xy_to_square(xx, yy))
            if p is not None:
                if piece_color(p) == by_color and piece_type(p) in ("B", "Q"):
                    return True
                break
            xx += dx
            yy += dy

    # Rooks / Queens
    for dx, dy in [(1, 0), (-1, 0), (0, 1), (0, -1)]:
        xx, yy = x + dx, y + dy
        while on_board(xx, yy):
            p = pos.piece_at(xy_to_square(xx, yy))
            if p is not None:
                if piece_color(p) == by_color and piece_type(p) in ("R", "Q"):
                    return True
                break
            xx += dx
            yy += dy

    # Kings
    for dx in (-1, 0, 1):
        for dy in (-1, 0, 1):
            if dx == 0 and dy == 0:
                continue
            xx, yy = x + dx, y + dy
            if on_board(xx, yy):
                p = pos.piece_at(xy_to_square(xx, yy))
                if p == ("K" if by_color == "w" else "k"):
                    return True

    return False


def in_check(pos, color):
    ksq = pos.king_square(color)
    return is_square_attacked(pos, ksq, opponent(color))


def pseudo_moves_for_piece(pos, from_sq):
    piece = pos.piece_at(from_sq)
    if piece is None:
        return []

    color = piece_color(piece)
    x, y = square_to_xy(from_sq)
    out = []

    def add(to_sq, promotion=None, is_ep=False, is_castle=False):
        out.append({
            "from": from_sq,
            "to": to_sq,
            "promotion": promotion,
            "is_ep": is_ep,
            "is_castle": is_castle,
        })

    pt = piece_type(piece)

    if pt == "P":
        direction = 1 if color == "w" else -1
        start_rank = 1 if color == "w" else 6
        promo_rank = 7 if color == "w" else 0

        # single push
        yy = y + direction
        if on_board(x, yy):
            to_sq = xy_to_square(x, yy)
            if pos.piece_at(to_sq) is None:
                if yy == promo_rank:
                    for pr in "qrbn":
                        add(to_sq, promotion=pr)
                else:
                    add(to_sq)

                # double push
                if y == start_rank:
                    yy2 = y + 2 * direction
                    to_sq2 = xy_to_square(x, yy2)
                    if pos.piece_at(to_sq2) is None:
                        add(to_sq2)

        # captures
        for dx in (-1, 1):
            xx, yy = x + dx, y + direction
            if not on_board(xx, yy):
                continue
            to_sq = xy_to_square(xx, yy)
            target = pos.piece_at(to_sq)
            if target is not None and piece_color(target) != color:
                if yy == promo_rank:
                    for pr in "qrbn":
                        add(to_sq, promotion=pr)
                else:
                    add(to_sq)

        # en passant
        if pos.en_passant != "-":
            ex, ey = square_to_xy(pos.en_passant)
            if ey == y + direction and abs(ex - x) == 1:
                add(pos.en_passant, is_ep=True)

    elif pt == "N":
        for dx, dy in [(1, 2), (2, 1), (2, -1), (1, -2),
                       (-1, -2), (-2, -1), (-2, 1), (-1, 2)]:
            xx, yy = x + dx, y + dy
            if on_board(xx, yy):
                to_sq = xy_to_square(xx, yy)
                target = pos.piece_at(to_sq)
                if target is None or piece_color(target) != color:
                    add(to_sq)

    elif pt in ("B", "R", "Q"):
        directions = []
        if pt in ("B", "Q"):
            directions += [(1, 1), (1, -1), (-1, 1), (-1, -1)]
        if pt in ("R", "Q"):
            directions += [(1, 0), (-1, 0), (0, 1), (0, -1)]
        for dx, dy in directions:
            xx, yy = x + dx, y + dy
            while on_board(xx, yy):
                to_sq = xy_to_square(xx, yy)
                target = pos.piece_at(to_sq)
                if target is None:
                    add(to_sq)
                else:
                    if piece_color(target) != color:
                        add(to_sq)
                    break
                xx += dx
                yy += dy

    elif pt == "K":
        for dx in (-1, 0, 1):
            for dy in (-1, 0, 1):
                if dx == 0 and dy == 0:
                    continue
                xx, yy = x + dx, y + dy
                if on_board(xx, yy):
                    to_sq = xy_to_square(xx, yy)
                    target = pos.piece_at(to_sq)
                    if target is None or piece_color(target) != color:
                        add(to_sq)

        # castling
        if color == "w" and from_sq == "e1":
            if "K" in pos.castling:
                if (pos.piece_at("f1") is None and pos.piece_at("g1") is None and
                    not in_check(pos, "w") and
                    not is_square_attacked(pos, "f1", "b") and
                    not is_square_attacked(pos, "g1", "b")):
                    add("g1", is_castle=True)
            if "Q" in pos.castling:
                if (pos.piece_at("d1") is None and pos.piece_at("c1") is None and pos.piece_at("b1") is None and
                    not in_check(pos, "w") and
                    not is_square_attacked(pos, "d1", "b") and
                    not is_square_attacked(pos, "c1", "b")):
                    add("c1", is_castle=True)

        if color == "b" and from_sq == "e8":
            if "k" in pos.castling:
                if (pos.piece_at("f8") is None and pos.piece_at("g8") is None and
                    not in_check(pos, "b") and
                    not is_square_attacked(pos, "f8", "w") and
                    not is_square_attacked(pos, "g8", "w")):
                    add("g8", is_castle=True)
            if "q" in pos.castling:
                if (pos.piece_at("d8") is None and pos.piece_at("c8") is None and pos.piece_at("b8") is None and
                    not in_check(pos, "b") and
                    not is_square_attacked(pos, "d8", "w") and
                    not is_square_attacked(pos, "c8", "w")):
                    add("c8", is_castle=True)

    return out


def make_move(pos, move):
    newpos = pos.copy()
    frm = move["from"]
    to = move["to"]
    promo = move.get("promotion")
    is_ep = move.get("is_ep", False)
    is_castle = move.get("is_castle", False)

    piece = newpos.piece_at(frm)
    target = newpos.piece_at(to)
    color = piece_color(piece)

    # update halfmove clock
    if piece_type(piece) == "P" or target is not None or is_ep:
        newpos.halfmove = 0
    else:
        newpos.halfmove += 1

    # remove en passant by default
    newpos.en_passant = "-"

    # move piece
    newpos.remove_piece(frm)

    # en passant capture
    if is_ep:
        fx, fy = square_to_xy(frm)
        tx, ty = square_to_xy(to)
        cap_sq = xy_to_square(tx, fy)
        newpos.remove_piece(cap_sq)

    # castling rook move
    if is_castle and piece_type(piece) == "K":
        if frm == "e1" and to == "g1":
            newpos.remove_piece("h1")
            newpos.set_piece("f1", "R")
        elif frm == "e1" and to == "c1":
            newpos.remove_piece("a1")
            newpos.set_piece("d1", "R")
        elif frm == "e8" and to == "g8":
            newpos.remove_piece("h8")
            newpos.set_piece("f8", "r")
        elif frm == "e8" and to == "c8":
            newpos.remove_piece("a8")
            newpos.set_piece("d8", "r")

    # promotion
    if promo:
        promoted = promo.upper() if color == "w" else promo.lower()
        newpos.set_piece(to, promoted)
    else:
        newpos.set_piece(to, piece)

    # double pawn push => en passant square
    if piece_type(piece) == "P":
        fx, fy = square_to_xy(frm)
        tx, ty = square_to_xy(to)
        if abs(ty - fy) == 2:
            ep_rank = (fy + ty) // 2
            newpos.en_passant = xy_to_square(fx, ep_rank)

    # castling rights
    def strip(chars):
        for ch in chars:
            newpos.castling = newpos.castling.replace(ch, "")

    if piece == "K":
        strip("KQ")
    elif piece == "k":
        strip("kq")
    elif frm == "a1" or to == "a1":
        strip("Q")
    elif frm == "h1" or to == "h1":
        strip("K")
    elif frm == "a8" or to == "a8":
        strip("q")
    elif frm == "h8" or to == "h8":
        strip("k")

    # also remove rights if rook captured on its home square
    if to == "a1":
        strip("Q")
    elif to == "h1":
        strip("K")
    elif to == "a8":
        strip("q")
    elif to == "h8":
        strip("k")

    # switch side
    newpos.side_to_move = opponent(pos.side_to_move)
    if pos.side_to_move == "b":
        newpos.fullmove += 1

    return newpos


def legal_moves(pos):
    color = pos.side_to_move
    out = []
    for sq, piece in list(pos.board.items()):
        if piece_color(piece) != color:
            continue
        for mv in pseudo_moves_for_piece(pos, sq):
            test = make_move(pos, mv)
            if not in_check(test, color):
                out.append(mv)
    return out


def move_matches_uci(move, frm, to, promo):
    if move["from"] != frm or move["to"] != to:
        return False
    mp = move.get("promotion")
    return (mp or None) == (promo or None)


def is_capture(pos, move):
    if move.get("is_ep"):
        return True
    return pos.piece_at(move["to"]) is not None


def san_for_move(pos, move):
    frm = move["from"]
    to = move["to"]
    piece = pos.piece_at(frm)
    pt = piece_type(piece)

    if move.get("is_castle"):
        san = "O-O" if to[0] == "g" else "O-O-O"
    else:
        capture = is_capture(pos, move)

        if pt == "P":
            san = ""
            if capture:
                san += frm[0] + "x"
            san += to
            if move.get("promotion"):
                san += "=" + move["promotion"].upper()
        else:
            san = pt

            # disambiguation
            candidates = []
            for mv in legal_moves(pos):
                if mv["to"] == to and mv["from"] != frm:
                    p2 = pos.piece_at(mv["from"])
                    if p2 and piece_type(p2) == pt:
                        candidates.append(mv)

            if candidates:
                same_file = any(mv["from"][0] == frm[0] for mv in candidates)
                same_rank = any(mv["from"][1] == frm[1] for mv in candidates)
                if not same_file:
                    san += frm[0]
                elif not same_rank:
                    san += frm[1]
                else:
                    san += frm

            if capture:
                san += "x"
            san += to

    newpos = make_move(pos, move)
    opp = newpos.side_to_move
    opp_moves = legal_moves(newpos)
    if in_check(newpos, opp):
        san += "#" if not opp_moves else "+"

    return san


def convert_to_pgn(input_text, result="*"):
    moves_uci, claimed_final_fen = parse_input(input_text)
    pos = Position.from_fen(START_FEN)

    san_moves = []

    for idx, (frm, to, promo) in enumerate(moves_uci, start=1):
        lm = legal_moves(pos)
        chosen = None
        for mv in lm:
            if move_matches_uci(mv, frm, to, promo):
                chosen = mv
                break

        if chosen is None:
            raise ValueError(
                f"Move {idx} is illegal or not found: {frm} {to}" +
                (f" {promo}" if promo else "")
            )

        san_moves.append(san_for_move(pos, chosen))
        pos = make_move(pos, chosen)

    if claimed_final_fen and pos.fen() != claimed_final_fen:
        raise ValueError(
            "Final FEN does not match computed position.\n"
            f"Computed: {pos.fen()}\n"
            f"Claimed:  {claimed_final_fen}"
        )

    lines = [
        '[Event "?"]',
        '[Site "?"]',
        '[Date "????.??.??"]',
        '[Round "?"]',
        '[White "?"]',
        '[Black "?"]',
        f'[Result "{result}"]',
        ""
    ]

    movetext_parts = []
    move_no = 1
    for i in range(0, len(san_moves), 2):
        if i + 1 < len(san_moves):
            movetext_parts.append(f"{move_no}. {san_moves[i]} {san_moves[i+1]}")
        else:
            movetext_parts.append(f"{move_no}. {san_moves[i]}")
        move_no += 1
    movetext_parts.append(result)

    movetext = " ".join(movetext_parts)

    wrapped = []
    line = ""
    for token in movetext.split():
        if len(line) + len(token) + (1 if line else 0) > 80:
            wrapped.append(line)
            line = token
        else:
            line = token if not line else line + " " + token
    if line:
        wrapped.append(line)

    return "\n".join(lines + wrapped)


if __name__ == "__main__":
    # This is the board that comes at at `SEARCH_DEPTH = 7` after looping
    raw = r"""
d2 d4, d7 d5, c2 c4, b8 c6, c4 d5, d8 d5, e2 e3, a8 b8, b1 c3, d5 d6, f2 f4, e8 d8, g1 f3, f7 f5, h1 g1, d6 d7, a2 a3, e7 e6, b2 b3, g8 e7, f1 b5, e7 d5, d1 d2, d7 e7, b5 c6, b7 c6, f3 e5, c8 b7, g2 g3, b7 a8, e3 e4, b8 b3, e4 d5, e6 d5, a3 a4, e7 e6, d2 c2, b3 b4, a1 b1, a7 a5, c3 a2, c6 c5, a2 b4, c5 b4, c2 e2, c7 c5, e2 b5, c5 d4, b5 a5, d8 e7, a5 a8, h8 g8, a8 a7, e7 d8, a7 b8, e6 c8, e5 f7, d8 d7, f7 e5, d7 d8, e5 f7, d8 d7, f7 e5, d7 d8, e5 f7, d8 d7, f7 e5, d7 d8, e5 f7, d8 d7, f7 e5, d7 d8, e5 f7, d8 d7, f7 e5, d7 d8, e5 f7, d8 d7, f7 e5, d7 d8, e5 f7, d8 d7, f7 e5, d7 d8, e5 f7, d8 d7, f7 e5, d7 d8, e5 f7, d8 d7, f7 e5, d7 d8, e5 f7, d8 d7, f7 e5, d7 d8, e5 f7, d8 d7, f7 e5, d7 d8, e5 f7, d8 d7, f7 e5, d7 d8, e5 f7, d8 d7, f7 e5, d7 d8, e5 f7, d8 d7, f7 e5, d7 d8, e5 f7, d8 d7, f7 e5, d7 d8, e5 f7, d8 d7, f7 e5, d7 d8, e5 f7, d8 d7, f7 e5, d7 d8, e5 f7, d8 d7, f7 e5, d7 d8, e5 f7, d8 d7, f7 e5, d7 d8, e5 f7, d8 d7, f7 e5, d7 d8, e5 f7, d8 d7, f7 e5, d7 d8, e5 f7, d8 d7, f7 e5, d7 d8, e5 f7, d8 d7, f7 e5, d7 d8, e5 f7, d8 d7, f7 e5, d7 d8,
1Qqk1br1/6pp/8/3pNp2/Pp1p1P2/6P1/7P/1RB1K1R1 w - - 101 77
"""
    print(convert_to_pgn(raw, result="*"))
