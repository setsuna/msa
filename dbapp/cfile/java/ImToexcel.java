import java.util.Calendar;
import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.util.Region;
import org.apache.poi.ss.usermodel.Font;
import org.apache.poi.ss.usermodel.IndexedColors;
import org.apache.poi.hssf.record.cf.BorderFormatting;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
public class ImToexcel{
	public static void main(String args[]) throws IOException{
		
		String group_name=args[0];
	    String user_name=args[1];
	    String servic_name=args[2];
	    String sDate=args[3];
	    String key_name=args[4];

		XLSCreate e=new XLSCreate("/datacenter/msa/report/msaexport/imrecord/imexport.xls");
        //字体
		    Font headfont = e.workbook.createFont();
		    headfont.setFontName("黑体");
		    headfont.setFontHeightInPoints((short) 22);// 字体大小
		    headfont.setBoldweight(Font.BOLDWEIGHT_BOLD);// 加粗
		    //标题
		    CellStyle headstyle = e.workbook.createCellStyle();
		    headstyle.setFont(headfont);
		    headstyle.setAlignment(CellStyle.ALIGN_CENTER);// 左右居中
		    headstyle.setVerticalAlignment(CellStyle.VERTICAL_CENTER);// 上下居中
		    headstyle.setLocked(true);
		    headstyle.setWrapText(true);// 自动换行
        //一个样式
		    CellStyle title = e.workbook.createCellStyle();
		    title.setBorderBottom(BorderFormatting.BORDER_THIN); // 设置单元格的边框为粗体
		    title.setBottomBorderColor(IndexedColors.BLACK.getIndex()); // 设置单元格的边框颜色．
		    title.setBorderTop(BorderFormatting.BORDER_THIN); // 设置单元格的边框为粗体
		    title.setTopBorderColor(IndexedColors.BLACK.getIndex()); // 设置单元格的边框颜色．
		    title.setBorderLeft(BorderFormatting.BORDER_THIN); // 设置单元格的边框为粗体
		    title.setLeftBorderColor(IndexedColors.BLACK.getIndex()); // 设置单元格的边框颜色．
		    title.setBorderRight(BorderFormatting.BORDER_THIN); // 设置单元格的边框为粗体
		    title.setRightBorderColor(IndexedColors.BLACK.getIndex()); // 设置单元格的边框颜色

				//字体
		    Font columnHeadFont = e.workbook.createFont();
		    columnHeadFont.setFontName("宋体");
		    columnHeadFont.setFontHeightInPoints((short) 10);
		    columnHeadFont.setBoldweight(Font.BOLDWEIGHT_BOLD);
		    // 列头的样式
		    CellStyle columnHeadStyle = e.workbook.createCellStyle();
		    columnHeadStyle.setFont(columnHeadFont);
		    columnHeadStyle.setAlignment(CellStyle.ALIGN_LEFT);// 左右居中
		    columnHeadStyle.setVerticalAlignment(CellStyle.VERTICAL_BOTTOM);// 上下居中
		    columnHeadStyle.setLocked(true);
		    columnHeadStyle.setWrapText(true);
		    
		    columnHeadStyle.setBorderBottom(BorderFormatting.BORDER_THIN); // 设置单元格的边框为粗体
		    columnHeadStyle.setBorderTop(BorderFormatting.BORDER_THIN); // 设置单元格的边框为粗体
		    columnHeadStyle.setBorderLeft(BorderFormatting.BORDER_THIN); // 设置单元格的边框为粗体．
		    columnHeadStyle.setBorderRight(BorderFormatting.BORDER_THIN); // 设置单元格的边框为粗体
		    //小标题
		    CellStyle titleStyle = e.workbook.createCellStyle();
		    titleStyle.setFont(columnHeadFont);
		    titleStyle.setAlignment(CellStyle.ALIGN_CENTER);// 左右居中
		    titleStyle.setVerticalAlignment(CellStyle.VERTICAL_CENTER);// 上下居中
		    titleStyle.setLocked(true);
		    titleStyle.setWrapText(true);
		    
		    titleStyle.setBorderBottom(BorderFormatting.BORDER_THIN); // 设置单元格的边框为粗体
		    titleStyle.setBorderTop(BorderFormatting.BORDER_THIN); // 设置单元格的边框为粗体
		    titleStyle.setBorderLeft(BorderFormatting.BORDER_THIN); // 设置单元格的边框为粗体．
		    titleStyle.setBorderRight(BorderFormatting.BORDER_THIN); // 设置单元格的边框为粗体
				// 普通单元格样式
			Font font = e.workbook.createFont();
		    font.setFontName("宋体");
		    font.setFontHeightInPoints((short) 10);
		    CellStyle style = e.workbook.createCellStyle();
		    style.setFont(font);
		    style.setAlignment(CellStyle.ALIGN_LEFT);// 左右居中
		    style.setVerticalAlignment(CellStyle.VERTICAL_BOTTOM);// 上下居中
		    style.setWrapText(true);
		    style.setLeftBorderColor(IndexedColors.BLACK.getIndex());
		    style.setBorderLeft((short) 1);
		    style.setRightBorderColor(IndexedColors.BLACK.getIndex());
		    style.setBorderRight((short)1);
		    style.setBorderLeft(BorderFormatting.BORDER_THIN); // 设置单元格的边框为粗体
		    style.setBorderRight(BorderFormatting.BORDER_THIN); // 设置单元格的边框为粗体
		    style.setBottomBorderColor(IndexedColors.BLACK.getIndex()); // 设置单元格的边框颜色
		    style.setFillForegroundColor(IndexedColors.WHITE.getIndex());// 设置单元格的背景颜色
		    
	        e.createRow(0,500);//第一行，并设置高度
	        e.mergingCell(0,0,0,8);//合并0到8
	        Region region = new Region((short)0,(short)0,(short)0,(short)8);
	        e.setRegionStyle(region,title);
	        e.setCell(0,"即时通信记录查询",headstyle);
	        
	        e.createRow(1);//第二行
	        e.mergingCell(1,1,1,8);
	        Region region1 = new Region((short)1,(short)1,(short)1,(short)8);
	        e.setRegionStyle(region1,title);
	        e.setCell(0,"查询日期",columnHeadStyle);
	        e.setCell(1,sDate,style);
	        
	        e.createRow(2);//第三行
	        e.mergingCell(2,2,0,8);
	        Region region2 = new Region((short)2,(short)0,(short)2,(short)8);
	        e.setRegionStyle(region2,title);
	        e.setCell(0,"过滤条件",titleStyle);
	        
	        e.createRow(3);//第四行
	        e.mergingCell(3,3,1,8);
	        Region region3 = new Region((short)3,(short)1,(short)3,(short)8);
	        e.setRegionStyle(region3,title);
	        e.setCell(0,"部门名称",columnHeadStyle);
	        e.setCell(1,group_name,style);
	        
	        e.createRow(4);//第五行
	        e.mergingCell(4,4,1,8);
	        Region region4= new Region((short)4,(short)1,(short)4,(short)8);
	        e.setRegionStyle(region4,title);
	        e.setCell(0,"用户名",columnHeadStyle);
	        e.setCell(1,user_name,style);
	        
	        e.createRow(5);//第六行
	        e.mergingCell(5,5,1,8);
	        Region region5= new Region((short)5,(short)1,(short)5,(short)8);
	        e.setRegionStyle(region5,title);
	        e.setCell(0,"通信类型",columnHeadStyle);
	        e.setCell(1,servic_name,style);
	        
	        e.createRow(6);//第六行
	        e.mergingCell(6,6,1,8);
	        Region region6= new Region((short)6,(short)1,(short)6,(short)8);
	        e.setRegionStyle(region6,title);
	        e.setCell(0,"关键字",columnHeadStyle);
	        e.setCell(1,key_name,style);
	        
	        e.createRow(7);//第八行
	        e.mergingCell(7,7,0,8);
	        Region region7= new Region((short)7,(short)0,(short)7,(short)8);
	        e.setRegionStyle(region7,title);
	        e.setCell(0,"查询结果",titleStyle);
	        
	        e.createRow(8);
	        e.setCell(0,"序号",columnHeadStyle,100);
	        e.setCell(1,"部门",columnHeadStyle,100);
	        e.setCell(2,"用户名",columnHeadStyle,100);
	        e.setCell(3,"访问日期",columnHeadStyle,100);
	        e.setCell(4,"访问时间",columnHeadStyle,100);
	        e.setCell(5,"发送",columnHeadStyle,100);
	        e.setCell(6,"接收",columnHeadStyle,100);
	        e.setCell(7,"访问内容",columnHeadStyle,100);
	        e.setCell(8,"通信类型",columnHeadStyle,100);
	        
	        int rownum=0;      
	        BufferedReader reader = null;
	        String line = null;
		    reader = new BufferedReader(new FileReader("/datacenter/msa/report/msaexport/imrecord/imexport.csv"));	     
		    while((line=reader.readLine())!=null){
		    	   String item[] = line.split("\\|");//将字符串分隔成字符数组
		    	   System.out.println(item.length);
		    	   e.createRow(rownum+9,250);     					
					  for (short cellnum = (short) 0; cellnum < item.length; cellnum++){  
						    System.out.println(item[cellnum]);
						    e.setCell(cellnum,item[cellnum],style);	
				      }		
					 rownum++;
					 if(rownum==20000) break;			  					  
		  }
		    
		   try {
			e.exportXLS();
		 } catch (Exception e1) {
			e1.printStackTrace();
		}
   }
}