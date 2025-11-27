from manim import *
import numpy as np
from copy import deepcopy

class ThreeSortsDashboardFixed2(Scene):
    def construct(self):
        values = ["89","23","55","67","19","0","5","42"]
        data_init = [int(x) for x in values]
        n = len(data_init)

        square_size = 0.9
        spacing = 0.35
        top_y = 2.8
        mid_y = 0.0
        bot_y = -2.8
        label_font = 24
        value_font = int(square_size * 40)
        swap_font = 22

        green_light = "#72C772"     
        python_blue = "#3776AB"     
        python_yellow = "#FFD43B"   
        red_light = "#F8C9C9"
        default_fill = BLACK
        default_text_color = WHITE

        def shell_swaps(arr):
            a = arr[:]
            swaps = []
            gap = len(a) // 2
            while gap > 0:
                for i in range(gap, len(a)):
                    temp = a[i]
                    j = i
                    while j >= gap and a[j - gap] > temp:
                        swaps.append((j - gap, j))
                        a[j] = a[j - gap]
                        j -= gap
                    a[j] = temp
                gap //= 2
            return swaps

        def insertion_swaps(arr):
            a = arr[:]
            swaps = []
            for i in range(1, len(a)):
                j = i
                while j > 0 and a[j-1] > a[j]:
                    swaps.append((j-1, j))
                    a[j-1], a[j] = a[j], a[j-1]
                    j -= 1
            return swaps

        def bubble_swaps(arr):
            a = arr[:]
            swaps = []
            n = len(a)
            for i in range(n):
                swapped = False
                for j in range(0, n - i - 1):
                    if a[j] > a[j+1]:
                        swaps.append((j, j+1))
                        a[j], a[j+1] = a[j+1], a[j]
                        swapped = True
                if not swapped:
                    break
            return swaps

        swaps_shell = shell_swaps(data_init)
        swaps_insertion = insertion_swaps(data_init)
        swaps_bubble = bubble_swaps(data_init)
        max_steps = max(len(swaps_shell), len(swaps_insertion), len(swaps_bubble))

        def make_row(y_pos, name_text, color_label=WHITE):
            squares = VGroup()
            for i, val in enumerate(values):
                sq = Square(side_length=square_size, stroke_width=2)
                sq.set_fill(default_fill, opacity=1.0)
                label = Text(val, font_size=value_font, color=default_text_color)
                grp = VGroup(sq, label)
                label.move_to(sq.get_center())
                squares.add(grp)
            squares.arrange(RIGHT, buff=spacing).move_to(ORIGIN).shift(UP * y_pos)
            title = Text(name_text, font_size=label_font, color=color_label).next_to(squares, UP, buff=0.2)
            swap_counter = Integer(0, font_size=swap_font, color=WHITE).next_to(squares, DOWN, buff=0.2)
            swap_label = Text("Swaps:", font_size=swap_font, color=WHITE).next_to(swap_counter, LEFT, buff=0.2)
            group = VGroup(squares, title, swap_label, swap_counter)
            return group, squares, title, swap_label, swap_counter

        top_group, top_squares, top_title, top_swap_label, top_swaps_num = make_row(top_y, "Shell Sort")
        mid_group, mid_squares, mid_title, mid_swap_label, mid_swaps_num = make_row(mid_y, "Insertion Sort")
        bot_group, bot_squares, bot_title, bot_swap_label, bot_swaps_num = make_row(bot_y, "Bubble Sort")

        self.play(FadeIn(top_group), FadeIn(mid_group), FadeIn(bot_group))
        self.wait(0.2)

        top_positions = [np.array(p) for p in [top_squares[i].get_center() for i in range(n)]]
        mid_positions = [np.array(p) for p in [mid_squares[i].get_center() for i in range(n)]]
        bot_positions = [np.array(p) for p in [bot_squares[i].get_center() for i in range(n)]]

        def prepare_swap_anims(squares_row, positions, left_idx, right_idx):
            left_grp = squares_row[left_idx]
            right_grp = squares_row[right_idx]
            self.bring_to_front(left_grp)
            self.bring_to_front(right_grp)
            p1 = left_grp.get_top()
            p2 = right_grp.get_top()
            arc = ArcBetweenPoints(p1, p2, angle=-PI/4, color=YELLOW)
            pre_anims = [
                Create(arc),
                left_grp[0].animate.set_fill(red_light, opacity=0.6),
                right_grp[0].animate.set_fill(red_light, opacity=0.6),
            ]
            move_anims = [
                left_grp.animate.move_to(positions[right_idx]),
                right_grp.animate.move_to(positions[left_idx]),
            ]
            post_anims = [
                left_grp[0].animate.set_fill(default_fill, opacity=1.0),
                right_grp[0].animate.set_fill(default_fill, opacity=1.0),
                FadeOut(arc),
            ]
            def post_fix():
                left_grp.move_to(positions[right_idx])
                right_grp.move_to(positions[left_idx])
                left_grp[1].move_to(left_grp[0].get_center())
                right_grp[1].move_to(right_grp[0].get_center())
            return pre_anims, move_anims, post_anims, post_fix

        top_order = [top_squares[i] for i in range(n)]
        mid_order = [mid_squares[i] for i in range(n)]
        bot_order = [bot_squares[i] for i in range(n)]

        if len(swaps_shell) == 0:
            self.play(*[g[0].animate.set_fill(green_light, opacity=0.8) for g in top_order], run_time=0.6)
        if len(swaps_insertion) == 0:
            self.play(*[g[0].animate.set_fill(green_light, opacity=0.8) for g in mid_order], run_time=0.6)
        if len(swaps_bubble) == 0:
            self.play(*[g[0].animate.set_fill(green_light, opacity=0.8) for g in bot_order], run_time=0.6)

        for step in range(max_steps):
            pre_anims_all = []
            move_anims_all = []
            post_anims_all = []
            post_fix_fns = []
            swaps_done = {"top": None, "mid": None, "bot": None}

            if step < len(swaps_shell):
                a, b = swaps_shell[step]
                pre, move, post, fix_fn = prepare_swap_anims(top_order, top_positions, a, b)
                pre_anims_all += pre
                move_anims_all += move
                post_anims_all += post
                post_fix_fns.append(fix_fn)
                swaps_done["top"] = (a, b)

            if step < len(swaps_insertion):
                a, b = swaps_insertion[step]
                pre, move, post, fix_fn = prepare_swap_anims(mid_order, mid_positions, a, b)
                pre_anims_all += pre
                move_anims_all += move
                post_anims_all += post
                post_fix_fns.append(fix_fn)
                swaps_done["mid"] = (a, b)

            if step < len(swaps_bubble):
                a, b = swaps_bubble[step]
                pre, move, post, fix_fn = prepare_swap_anims(bot_order, bot_positions, a, b)
                pre_anims_all += pre
                move_anims_all += move
                post_anims_all += post
                post_fix_fns.append(fix_fn)
                swaps_done["bot"] = (a, b)

            if pre_anims_all:
                self.play(*pre_anims_all, run_time=0.28)

            if move_anims_all:
                self.play(*move_anims_all, run_time=0.7)

            if swaps_done["top"] is not None:
                a, b = swaps_done["top"]
                top_order[a], top_order[b] = top_order[b], top_order[a]
                top_swaps_num.increment_value()
            if swaps_done["mid"] is not None:
                a, b = swaps_done["mid"]
                mid_order[a], mid_order[b] = mid_order[b], mid_order[a]
                mid_swaps_num.increment_value()
            if swaps_done["bot"] is not None:
                a, b = swaps_done["bot"]
                bot_order[a], bot_order[b] = bot_order[b], bot_order[a]
                bot_swaps_num.increment_value()

            if post_anims_all:
                self.play(*post_anims_all, run_time=0.18)

            for fn in post_fix_fns:
                fn()

            for i in range(n):
                top_order[i].move_to(top_positions[i])
                top_order[i][1].move_to(top_order[i][0].get_center())
                mid_order[i].move_to(mid_positions[i])
                mid_order[i][1].move_to(mid_order[i][0].get_center())
                bot_order[i].move_to(bot_positions[i])
                bot_order[i][1].move_to(bot_order[i][0].get_center())

            if step == len(swaps_shell) - 1 and len(swaps_shell) > 0:
                self.play(*[g[0].animate.set_fill(green_light, opacity=0.8) for g in top_order], run_time=0.6)
            if step == len(swaps_insertion) - 1 and len(swaps_insertion) > 0:
                self.play(*[g[0].animate.set_fill(green_light, opacity=0.8) for g in mid_order], run_time=0.6)
            if step == len(swaps_bubble) - 1 and len(swaps_bubble) > 0:
                self.play(*[g[0].animate.set_fill(green_light, opacity=0.8) for g in bot_order], run_time=0.6)

            self.wait(0.06)

        self.wait(0.3)

        center_pos = ORIGIN + DOWN * 0.4
        self.play(
            top_swap_label.animate.move_to(center_pos + LEFT * 0.6),
            bot_swap_label.animate.move_to(center_pos + RIGHT * 0.6),
            mid_swap_label.animate.move_to(center_pos),
            run_time=0.6
        )
        self.play(FadeOut(top_swap_label), FadeOut(bot_swap_label), run_time=0.35)
        swaps_axis_label = Text("Swaps", font_size=swap_font+2, color=WHITE)
        swaps_axis_label.move_to(mid_swap_label.get_center())
        self.play(Transform(mid_swap_label, swaps_axis_label), run_time=0.3)

        to_fade = VGroup()
        to_fade.add(*top_order)
        to_fade.add(*mid_order)
        to_fade.add(*bot_order)
        self.play(FadeOut(to_fade), run_time=0.8)
        self.wait(0.2)

        chart_base_y = -1.2 
        chart_height = 3.0
        x_positions = [-2.4, 0.0, 2.4]
        max_swaps = max(int(top_swaps_num.get_value()), int(mid_swaps_num.get_value()), int(bot_swaps_num.get_value()), 1)

        x_len = 6.5
        x_start = np.array([-x_len/2, chart_base_y, 0.0])  
        x_end = np.array([ x_len/2, chart_base_y, 0.0])   
        x_axis = Line(x_start, x_end, color=WHITE)
        y_len = 4.5
        y_axis = Line(x_start + DOWN * 0.2, x_start + UP * (y_len - 0.2), color=WHITE)

        self.play(Create(x_axis), Create(y_axis))
        self.wait(0.12)

        swaps_label_for_axis = swaps_axis_label.copy()
        swaps_label_for_axis.move_to(x_start + UP * (y_len/2) + LEFT * 0.8)
        swaps_label_for_axis.rotate(PI/2)
        self.play(Transform(mid_swap_label, swaps_label_for_axis), run_time=0.4)

        titles = [top_title, mid_title, bot_title]
        counters = [top_swaps_num, mid_swaps_num, bot_swaps_num]
        for i, x in enumerate(x_positions):
            self.play(titles[i].animate.move_to(np.array([x, chart_base_y - 0.6, 0])), run_time=0.35)
            self.play(counters[i].animate.move_to(np.array([x, chart_base_y + chart_height/2 + 0.35, 0])), run_time=0.35)

        self.wait(0.12)

        bar_width = 1.0
        bar_colors = [green_light, python_blue, python_yellow]
        bars_group = VGroup()
        for i, x in enumerate(x_positions):
            val = int(counters[i].get_value())
            height = (val / max_swaps) * chart_height if max_swaps > 0 else 0.1
            bar_center_y = chart_base_y + height / 2
            bar_final = Rectangle(width=bar_width, height=height, fill_color=bar_colors[i], fill_opacity=0.95, stroke_width=1)
            bar_final.move_to(np.array([x, bar_center_y, 0]))
            start_bar = Rectangle(width=bar_width, height=0.01, fill_color=bar_colors[i], fill_opacity=0.95, stroke_width=0)
            start_bar.move_to(np.array([x, chart_base_y + 0.01/2, 0]))
            bars_group.add(VGroup(start_bar, bar_final))

        transforms = [Transform(grp[0], grp[1], run_time=0.9) for grp in bars_group]
        self.play(*transforms)
        self.wait(0.18)

        for i, x in enumerate(x_positions):
            val = int(counters[i].get_value())
            height = (val / max_swaps) * chart_height if max_swaps > 0 else 0.1
            new_counter_pos = np.array([x, chart_base_y + height + 0.25, 0])
            self.play(counters[i].animate.move_to(new_counter_pos), run_time=0.25)

        self.wait(1.0)
