from manim import *
import os 

class ShellSortDashboard(Scene):
    def construct(self):
        script_content = """def shell_sort(arr):
    n = len(arr)
    gap = n // 2
    while gap > 0:
        for i in range(gap, n):
            temp = arr[i]
            j = i
            while j >= gap and arr[j-gap] > temp:
                arr[j] = arr[j-gap]
                j -= gap
            arr[j] = temp
        gap //= 2"""
        
        filename = "shell_temp.py"
        with open(filename, "w") as f:
            f.write(script_content)

        n = 8
        tamanoD = 0.9                     
        square_size = tamanoD             
        inicio_size = tamanoD * 1.45
        top_y = 2.5  
        spacing = 0.4
        
        default_fill = BLACK
        default_text_color = WHITE
        red_light = "#F8C9C9"
        lvl3_color = "#72C772" 
        
        values = ["89","23","55","67","19","0","5","42"]
        data = [int(x) for x in values] 

        starter_sq = Square(side_length=inicio_size, stroke_width=2)
        starter_label = Text("0", font_size=int(inicio_size*40), color=WHITE)
        starter = VGroup(starter_sq, starter_label)
        starter_label.move_to(starter_sq.get_center())
        self.play(FadeIn(starter))
        self.wait(0.28)

        shell_text = Text("SHELL SORT", font_size=48)
        shell_text.next_to(starter, UP, buff=0.12)
        self.play(Write(shell_text))
        self.wait(0.18)

        self.play(shell_text.animate.scale(0.6).to_edge(UL))
        self.wait(0.12)

        scale_to_default = square_size / inicio_size
        self.play(starter.animate.scale(scale_to_default))
        self.wait(0.12)

        tmp = VGroup(*[Square(side_length=square_size) for _ in range(n)])
        tmp.arrange(RIGHT, buff=spacing).move_to(ORIGIN).shift(UP * top_y)
        top_positions = [tmp[i].get_center() for i in range(n)]

        self.play(starter.animate.move_to(top_positions[5]), run_time=0.45)
        self.wait(0.12)

        top_squares = VGroup()
        for i in range(n):
            sq = Square(side_length=square_size, stroke_width=2)
            sq.set_fill(default_fill, opacity=1.0)
            label = Text(values[i], font_size=int(square_size*45), color=default_text_color) 
            label.move_to(sq.get_center())
            grp = VGroup(sq, label).move_to(top_positions[i])
            top_squares.add(grp)

        self.play(ReplacementTransform(starter, top_squares[5]), run_time=0.16)
        
        create_order = [4,3,2,1,0,6,7] 
        rest_squares = [top_squares[i] for i in create_order if i != 5]
        
        final_display_order = [top_squares[i] for i in create_order]
        
        self.play(LaggedStart(*[FadeIn(top_squares[i]) for i in range(n) if i != 5], lag_ratio=0.05), run_time=0.9)

        idx_labels = VGroup()
        for i, grp in enumerate(top_squares):
            idx = Text(str(i), font_size=16, color=default_text_color).next_to(grp, UP, buff=0.1)
            idx_labels.add(idx)
        
        self.play(Write(idx_labels))

        code_obj = Code(
            filename, 
            language="python",
            background="window"
        )
        
        code_obj.scale(0.65).to_corner(DR, buff=0.5)
        
        CODE_LINES = code_obj[1]

        highlight = SurroundingRectangle(CODE_LINES[0], color=YELLOW, fill_opacity=0.2, stroke_width=2)
        
        self.play(FadeIn(code_obj), Create(highlight))

        gap_text = Text("GAP: ", font_size=30, color=YELLOW).to_edge(LEFT, buff=1).shift(UP*0.5)
        gap_num = Integer(0, font_size=30, color=YELLOW).next_to(gap_text, RIGHT)
        
        comp_title = Text("Complejidad:", font_size=24, color=BLUE).next_to(gap_text, DOWN, buff=0.8, aligned_edge=LEFT)
        comp_val = MathTex(r"O(n \log^2 n)", font_size=24).next_to(comp_title, RIGHT)
        
        swaps_title = Text("Swaps:", font_size=24, color=WHITE).next_to(comp_title, DOWN, buff=0.4, aligned_edge=LEFT)
        swaps_num = Integer(0, font_size=24, color=WHITE).next_to(swaps_title, RIGHT)

        info_group = VGroup(gap_text, gap_num, comp_title, comp_val, swaps_title, swaps_num)
        self.play(Write(info_group))

        def move_hl(line_index, run_time=0.1):
            line = CODE_LINES[line_index]
            return highlight.animate.move_to(line.get_center()).stretch_to_fit_width(line.width + 0.2).stretch_to_fit_height(line.height + 0.1)

        n_len = len(data)
        
        self.play(move_hl(2)) 
        gap = n_len // 2
        self.play(gap_num.animate.set_value(gap))

        self.play(move_hl(3))

        while gap > 0:
            self.play(move_hl(4))
            
            for i in range(gap, n_len):
                temp_val = data[i]
                j = i
                
                pivot_sq = top_squares[j][0]
                self.play(pivot_sq.animate.set_stroke(YELLOW, width=6), run_time=0.2)

                self.play(move_hl(7), run_time=0.1)

                while j >= gap and data[j - gap] > temp_val:
                    self.play(move_hl(8), run_time=0.1)
                    swaps_num.increment_value() 

                    left_idx = j - gap
                    right_idx = j

                    p1 = top_squares[left_idx].get_top()
                    p2 = top_squares[right_idx].get_top()
                    arc = ArcBetweenPoints(p1, p2, angle=-PI/4, color=YELLOW)
                    self.play(Create(arc), run_time=0.3)
                    
                    self.play(
                        top_squares[left_idx][0].animate.set_fill(red_light, opacity=0.5),
                        top_squares[right_idx][0].animate.set_fill(red_light, opacity=0.5),
                        run_time=0.2
                    )

                    self.play(
                        top_squares[left_idx].animate.move_to(top_positions[right_idx]),
                        top_squares[right_idx].animate.move_to(top_positions[left_idx]),
                        run_time=0.4
                    )
                    top_squares[left_idx], top_squares[right_idx] = top_squares[right_idx], top_squares[left_idx]

                    self.play(
                        top_squares[left_idx][0].animate.set_fill(default_fill, opacity=1),
                        top_squares[right_idx][0].animate.set_fill(default_fill, opacity=1),
                        FadeOut(arc),
                        run_time=0.2
                    )

                    data[j] = data[j - gap]
                    j -= gap
                    data[j] = temp_val
                    
                    self.play(move_hl(7), run_time=0.1)

                self.play(move_hl(10), run_time=0.1)
                self.play(top_squares[j][0].animate.set_stroke(WHITE, width=2), run_time=0.1)
            
            self.play(move_hl(11))
            gap //= 2
            self.play(gap_num.animate.set_value(gap))
            
            self.play(move_hl(3))

        self.play(
            *[sq[0].animate.set_fill(lvl3_color, opacity=0.8) for sq in top_squares],
            run_time=1.5
        )
        self.wait(2)

        try:
            os.remove(filename)
        except:
            pass
