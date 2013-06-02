import java.util.Calendar;
import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.util.Region;
import org.apache.poi.ss.usermodel.Font;
import org.apache.poi.ss.usermodel.IndexedColors;
import org.apache.poi.hssf.record.cf.BorderFormatting;
import java.io.BufferedReader;
import java.io.FileReader;
public   class  IplogConvert{
    static   void  main(String args[])throws Exception{
    		String path=args[0];
    		String fileName=args[1];
    		int i=0;
    		String group="";
        XLSCreate e=new XLSCreate("/app/ns/java/poi/convExcel/"+fileName+".xls");
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

        e.createRow(0,500);
        e.mergingCell(0,0,0,20);
        Region region = new Region((short)0,(short)0,(short)0,(short)20);
        e.setRegionStyle(region,title);
        e.setCell(0,"服务访问记录查询",headstyle);
        e.createRow(1);
        e.mergingCell(1,1,1,20);
        Region region1 = new Region((short)1,(short)0,(short)1,(short)20);
        e.setRegionStyle(region1,title);
        e.setCell(0,"报表名称",columnHeadStyle);
        e.setCell(1,"服务访问记录查询",style);
        e.createRow(2);
        e.mergingCell(2,2,0,21);
        Region region2 = new Region((short)2,(short)0,(short)2,(short)20);
        e.setRegionStyle(region2,title);
        e.setCell(0,"查询结果",titleStyle);
        e.createRow(3);
        e.setCell(0,"序号",columnHeadStyle,100);
        e.setCell(1,"Userid",columnHeadStyle,100);
        e.setCell(2,"用户名",columnHeadStyle,100);
        e.setCell(3,"显示名",columnHeadStyle,100);
        e.setCell(4,"部门",columnHeadStyle,100);
        e.setCell(5,"传输方向",columnHeadStyle,100);
        e.setCell(6,"协议",columnHeadStyle,100);
        e.setCell(7,"源IP",columnHeadStyle,100);
        e.setCell(8,"自身MAC",columnHeadStyle,100);
        e.setCell(9,"目的IP",columnHeadStyle,100);
        e.setCell(10,"源端口",columnHeadStyle,100);
        e.setCell(11,"目的端口",columnHeadStyle,100);
        e.setCell(12,"开始连接日期",columnHeadStyle,100);
        e.setCell(13,"开始连接时间",columnHeadStyle,100);
        e.setCell(14,"上行流量",columnHeadStyle,100);
        e.setCell(15,"下行流量",columnHeadStyle,100);
        e.setCell(16,"数据包数目",columnHeadStyle,100);
        e.setCell(17,"一级服务",columnHeadStyle,100);
        e.setCell(18,"二级服务",columnHeadStyle,100);
        e.setCell(19,"虚拟通道",columnHeadStyle,100);
        e.setCell(20,"关键字",columnHeadStyle,100);
				
        try{
        		BufferedReader reader = new BufferedReader(new FileReader(path));
		        reader.readLine();
		        String line = null;
		        while((line=reader.readLine())!=null){
		            String item[] = line.split(",");
		        		e.createRow(i+4,230);
					      e.setCell(0,String.valueOf(i+1),style);
		            for(int j=1;j<=item.length;j++){
					      	if(j==4){
					      		group=item[j]+"/"+item[j+1]+"/"+item[j+2]+"/"+item[j+3]+"/"+item[j+4];
					      		e.setCell(j,group,style);
					      	}else if(j>4){
					      		e.setCell(j,item[j+3],style);
					      	}else{
					      		e.setCell(j,item[j-1],style);
					      	}
					      }
		            //int value = Integer.parseInt(last);
		            i++;
						}
            e.exportXLS();
            System.out.println( " 导出Excel文件[成功]" );
        }catch(Exception e1){
            System.out.println( " 导出Excel文件[失败]" );
            e1.printStackTrace();
        }

    }
}
