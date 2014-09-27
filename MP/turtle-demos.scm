;; Some turtle demo code

(define (turtle-clear-and-center cid)
  (turtle-clear-instructions cid)
  (turtle-center cid))

(define (turtle-center cid)
  (let* ((width (component-get-width cid))
         (height (component-get-height cid))
         (position (turtle-get-position cid (turtle-get-num-steps cid)))
         (x (car position))
         (y (cadr position))
         (orientation (turtle-get-orientation cid (turtle-get-num-steps cid))))
    (turtle-rotate cid (- orientation))
    (if (< x 0)
        (turtle-forward cid x #f)
        (begin
          (turtle-rotate cid PI)
          (turtle-forward cid x #f)
          (turtle-rotate cid PI)))
    (if (< y 0)
        (turtle-forward cid y #f)
        (begin
          (turtle-rotate cid (- (/ PI 2)))
          (turtle-forward cid y #f)
          (turtle-rotate cid (/ PI 2))))
    (turtle-forward cid (/ width 2) #f)
    (turtle-rotate cid (/ PI 2))
    (turtle-forward cid (/ height 2) #f)
    (turtle-rotate cid PI)))

(define (turtle-draw-line cid length)
  (turtle-forward cid length #t))

(define (turtle-dotted-line cid length redraw-now)
  (define (draw-dot length-left draw)
    (if (<= length-left 0)
        (turtle-draw-point cid redraw-now)
        (begin
          (when draw
            (turtle-draw-point cid redraw-now))
          (turtle-forward cid 1 #f redraw-now)
          (draw-dot (- length-left 1) (not draw)))))
  (draw-dot length #t))

(define (turtle-draw-pivot cid length theta line-fn)
  (line-fn cid length #f)
  (turtle-rotate cid (- PI theta) #f))

;; This is broken for even numbers...
(define (turtle-star cid length num-points . args)
  (define (draw-star num-sides go-up)
    (let* ((theta (/ (* 2 PI) num-sides 2))
          (y-offset (/ (* length
                          (cos (/ theta 2)))
                       2))
          (line-fn (if (null? args) turtle-draw-line (car args))))
     (define (draw-side num-left)
       (unless (<= num-left 0)
         (turtle-draw-pivot cid length theta line-fn)
         (draw-side (- num-left 1))))
     (turtle-center cid)
     (unless go-up
       (turtle-rotate cid (/ PI num-sides)))
     (turtle-forward cid y-offset #f #f)
     (turtle-rotate cid (- PI (/ theta 2)) #f)
     (draw-side num-sides)
     (turtle-draw-all cid)))
  (turtle-clear-instructions cid)
  (if (even? num-points)
      (if (odd? (/ num-points 2))
          (let ((num-sides (/ num-points 2)))
            (draw-star num-sides #t)
            (draw-star num-sides #f))
          (draw-star (/ num-points 2) #t))
      (draw-star num-points #t))
  (turtle-broadcast-instructions-changed cid))

;; Once the application is loaded, you can call this to make a star
;; The 300 argument is the length of the edges of the star, in pixels
;; The 23 argument is the number of points for the star
;; (turtle-star (find-turtle-graphics-widget) 300 23)

