; Вспомогательные функции

(deffunction ask-question (?question $?allowed-values)
   (printout t ?question)
   (bind ?answer (read))
   (if (lexemep ?answer) 
       then (bind ?answer (lowcase ?answer)))
   (while (not (member ?answer ?allowed-values)) do
      (printout t ?question)
      (bind ?answer (read))
      (if (lexemep ?answer) 
          then (bind ?answer (lowcase ?answer))))
   ?answer)

; Функция задает пользователю вопрос и получает
; ответ yes(у) или; no(n). В случае положительного
; ответа функция возвращает значение TRUE, иначе - FALSE
(deffunction yes-or-no-p (?question)
   (bind ?response (ask-question ?question yes no y n))
   (if (or (eq ?response yes) (eq ?response y))
       then TRUE 
       else FALSE))

; Диагностические правила

; Правило определяющее прием пищи по ответам пользователя:
; обед или ужин.
(defrule determine-meal-type ""
   =>
   (bind ?response 
      (ask-question "Kakoj u vas priem pishchi: zavtrak(1) ili uzhin(2) ? " 1 2))
      (if (eq ?response 1)
        then 
	  (assert (meal-type zavtrak))
        else 
          (assert (meal-type uzhin))))

; Правило определяющее имееющееся время на готовку
; по ответам пользователя: достаточно или мало.
(defrule determine-time-need-breakfast ""
   =>
   (if (yes-or-no-p "U vas mnogo vremeni na gotovku (yes/no)? ") 
       then 
         (assert (time-need enough))
       else 
         (assert (time-need few))))

; Правило определяющее итоговое блюдо по необходимости
; добавления яиц, если это завтрак и мало времени на
; готовку, по ответам пользователя. Если яица не нужны
; определяется итоговое блюдо - бутерброды. Если
; нужны, то также определяется жарить их или варить
; по ответам пользователя, если жарить, то итоговое
; блюдо - яичница, иначе - салат с вареными яицами
(defrule determine-egg-need ""
   (meal-type zavtrak)	
   (time-need few)
   =>
   (if (yes-or-no-p "Hotite ispol'zovat' yajca (yes/no)? ") 
       then 
       (if (yes-or-no-p "Pozharit' yajca (yes/no)? ")
           then (assert (dish "yaichnica."))
           else (assert (dish "salat s varennymi yajcami.")))
       else 
       	   (assert (dish "buterbrody."))))

; Правило определяющее итоговое блюдо по вкусовым
; предпочтениям, если это завтрак и много времени на
; готовку, по ответам пользователя. Если сладкое,
; то итоговое блюдо - блины, иначе - ролл с ветчиной 
; и овощами
(defrule determine-taste-type ""
   (meal-type zavtrak)
   (time-need enough)
   =>
   (bind ?response 
      (ask-question "Hotite sladkoe(1) ili solenoe(2)? " 1 2))
      (if (eq ?response 1)
        then 
	  (assert (dish "bliny."))
        else 
          (assert (dish "roll s vetchinoj i ovoshchami."))))

; Правило определяющее итоговое блюдо по вкусовым
; предпочтениям, если это ужин и мало времени на
; готовку, по ответам пользователя. Если курица,
; то итоговое блюдо - паста с курицей, иначе - 
; жареная рыба с рисом
(defrule determine-ingredient-type
   (meal-type uzhin)
   (time-need few)
   =>
   (bind ?response 
      (ask-question "Hotite kuricu(1) ili rybu(2)? " 1 2))
      (if (eq ?response 1)
        then 
	  (assert (dish "pasta s kuricej."))
        else 
          (assert (dish "zharenaya ryba s risom."))))

; Правило определяющее итоговое блюдо по вкусовым
; предпочтениям, если это ужин и много времени на
; готовку, по ответам пользователя. Если не суп,
; то итоговое блюдо - запеченая курица, иначе  
; по ответам пользователя определяется использовать
; мясо или нет. Если да, итоговое блюдо - борщ,
; иначе - суп-пюре
(defrule determine-soup-need ""
   (meal-type uzhin)	
   (time-need enough)
   =>
   (if (yes-or-no-p "Hotite svarit' sup (yes/no)? ") 
       then 
       (if (yes-or-no-p "S myasom (yes/no)? ")
           then (assert (dish "borshch."))
           else (assert (dish "tykvennyj sup-pyure.")))
       else 
       (assert (dish "zapechennaya kurica s kartoshkoj i gribami."))))

; Запуск и завершение

(defrule system-banner ""
  (declare (salience 10))
  =>
  (printout t crlf crlf)
  (printout t "Expert dish selection system")
  (printout t crlf crlf))

(defrule print-suggest ""
  (declare (salience 10))
  (dish ?item)
  =>
  (printout t crlf crlf)
  (printout t "Luchshee blyudo dlya vas: ")
  (format t " %s%n%n%n" ?item))

