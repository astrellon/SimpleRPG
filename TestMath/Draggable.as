package
{
	import flash.display.Sprite;
	import flash.display.Graphics;
	import flash.geom.Point;
	import flash.events.MouseEvent;

	public class Draggable extends Sprite
	{
		public function Draggable(colour:uint, radius:Number, px:Number, py:Number)
		{
			super();
			var g:Graphics = this.graphics;
			g.clear();
			g.beginFill(colour);
			g.lineStyle(2);
			g.drawCircle(0, 0, radius);
			g.endFill();
			
			x = px;
			y = py;
			
			this.addEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
		}
		
		public function get position():Point
		{
			return new Point(x, y);
		}
		
		protected function onMouseDown(event:MouseEvent):void
		{
			this.stage.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
			this.stage.addEventListener(MouseEvent.MOUSE_UP, onMouseUp);
		}
		
		protected function onMouseMove(event:MouseEvent):void
		{
			this.x = event.stageX;
			this.y = event.stageY;
		}
		
		protected function onMouseUp(event:MouseEvent):void
		{
			this.stage.removeEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
			this.stage.removeEventListener(MouseEvent.MOUSE_UP, onMouseUp);
		}
	}
}