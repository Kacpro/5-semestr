import model.Photo;
import model.PhotoSize;
import util.PhotoDownloader;
import util.PhotoProcessor;
import util.PhotoSerializer;

import java.io.IOException;
import java.util.List;
import java.util.concurrent.TimeUnit;
import java.util.logging.Level;
import java.util.logging.Logger;
import io.reactivex.*;
import io.reactivex.observables.*;
import io.reactivex.schedulers.Schedulers;

public class PhotoCrawler {

    private static final Logger log = Logger.getLogger(PhotoCrawler.class.getName());

    private final PhotoDownloader photoDownloader;

    private final PhotoSerializer photoSerializer;

    private final PhotoProcessor photoProcessor;

    public PhotoCrawler() throws IOException {
        this.photoDownloader = new PhotoDownloader();
        this.photoSerializer = new PhotoSerializer("./photos");
        this.photoProcessor = new PhotoProcessor();
    }

    public void resetLibrary() throws IOException {
        photoSerializer.deleteLibraryContents();
    }

    public void downloadPhotoExamples() {
        try {
            Observable<Photo> downloadedExamples = photoDownloader.getPhotoExamples();
            downloadedExamples.subscribe(photo -> photoSerializer.savePhoto(photo));
        } catch (IOException e) {
            log.log(Level.SEVERE, "Downloading photo examples error", e);
        }
    }

    public void downloadPhotosForQuery(String query) throws IOException {
    	try {
	    	photoDownloader
    			.searchForPhotos(query)
    			.compose(transform)
    			.subscribe(group -> group.subscribe(photo -> photoSerializer.savePhoto(photo)));
    	}
    	catch (IOException e){
    		log.log(Level.SEVERE, "Downloading photo examples error", e);
    	}
    }
    
    public void downloadPhotosForMultipleQueries(List<String> queries) throws IOException {
    	try {
	    	Observable<GroupedObservable<Boolean, Photo>> photos = photoDownloader
	    			.searchForPhotos(queries)
	    			.compose(transform);
	    	photos.subscribe(group -> group.subscribe(p -> System.out.println(p)));
	    			photos.subscribe(group -> {
	    				if (group.getKey() == true) {
	    					group
	    					.buffer(5, TimeUnit.SECONDS)
	    					.flatMap(list -> Observable.fromIterable(list))
	    					.subscribe(photo -> photoSerializer.savePhoto(photo));
	    				}
	    				else {
	    					group
	    					.observeOn(Schedulers.computation())
	    					.subscribe(photo -> photoSerializer.savePhoto(photo));
	    				}
	    			});
    	}
    	catch (IOException e){
    		log.log(Level.SEVERE, "Downloading photo examples error", e);
    	}
    }
    
    public Observable<GroupedObservable<Boolean, Photo>> processPhotos(Observable<Photo> allPhotos){
    	allPhotos.subscribe(p -> System.out.println(PhotoSize.resolve(p)));
    	Observable<Photo> filteredPhotos = allPhotos
    			.filter(photo -> photoProcessor.isPhotoValid(photo));
    	filteredPhotos.subscribe(p -> System.out.println(p));
    	return filteredPhotos.groupBy(photo -> PhotoSize.resolve(photo) == PhotoSize.MEDIUM);
    }
    
    public ObservableTransformer<Photo, GroupedObservable<Boolean, Photo>> transform = photos -> 
    {
//    	photos.subscribe(p -> System.out.println(p));
    	return processPhotos(photos);
    };
    

}
